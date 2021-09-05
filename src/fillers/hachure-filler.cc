#include "hachure-filler.h"

#include "filler-interface.h"
#include "core.h"
#include "geometry.h"
#include "scan-line-hachure.h"
#include "vector-utils.h"

struct IntersectionInfo {
  Point point;
  double distance;
};


HachureFiller::HachureFiller(RenderHelper* helper) {
  this->helper = helper;
}

OpSet HachureFiller::fillPolygon(const std::vector<Point>& points, Options& o) {
  return this->_fillPolygon(points, o);
}

OpSet HachureFiller::_fillPolygon(const std::vector<Point>& points, Options& o, bool connectEnds) {
  auto lines = polygonHachureLines(points, o);
  if (connectEnds) {
    const auto connectingLines = this->connectingLines(points, lines);
    insert(&lines, connectingLines);
  }
  const auto ops = this->renderLines(lines, o);
  return OpSet{ OpSetType::fillSketch, ops };
}

std::vector<Op> HachureFiller::renderLines(const std::vector<Line>& lines, Options& o) {
  std::vector<Op> ops;
  for (const auto& line: lines) {
    insert(&ops, this->helper->doubleLineOps(line, o));
  }
  return ops;
}

std::vector<Line> HachureFiller::connectingLines(const std::vector<Point>& polygon, const std::vector<Line>& lines) {
  std::vector<Line> result;
  if (lines.size() > 1) {
    for (std::size_t i = 1; i < lines.size(); i++) {
      const auto prev = lines[i - 1];
      if (lineLength(prev) < 3) {
        continue;
      }
      const auto current = lines[i];
      const auto segment= Line{current.from, prev.to};
      if (lineLength(segment) > 3) {
        const auto segSplits = this->splitOnIntersections(polygon, segment);
        insert(&result, segSplits);
      }
    }
  }
  return result;
}

bool HachureFiller::midPointInPolygon(const std::vector<Point>& polygon, const Line& segment) {
  return isPointInPolygon(polygon, (segment.from.x + segment.to.x) / 2, (segment.from.y + segment.to.y) / 2);
}

std::vector<Line> HachureFiller::splitOnIntersections(const std::vector<Point>& polygon, const Line& segment) {
  const auto error = std::max(5.0, lineLength(segment) * 0.1);
  std::vector<IntersectionInfo> intersections;
  for (std::size_t i = 0; i < polygon.size(); i++) {
    const auto& p1 = polygon[i];
    const auto& p2 = polygon[(i + 1) % polygon.size()];
    if (doIntersect(p1, p2, segment)) {
      const auto ip = lineIntersection(p1, p2, segment.from, segment.to);
      if (ip) {
        const auto d0 = lineLength({*ip, segment.from});
        const auto d1 = lineLength({*ip, segment.to});
        if (d0 > error && d1 > error) {
          intersections.emplace_back(IntersectionInfo{*ip, d0});
        }
      }
    }
  }
  if (intersections.size() > 1) {
    const auto sorted_ips = sorted(intersections, [](const IntersectionInfo& a, const IntersectionInfo& b) {return a.distance - b.distance;});
    auto ips = map<Point>(sorted_ips, [](const IntersectionInfo& d) { return d.point; });
    
    if (!isPointInPolygon(polygon, segment.from)) {
      pop_front(&ips);
    }
    if (!isPointInPolygon(polygon, segment.to)) {
      ips.pop_back();
    }
    if (ips.size() <= 1) {
      if (this->midPointInPolygon(polygon, segment)) {
        return {segment};
      } else {
        return {};
      }
    }
    auto spoints = std::vector<Point>{segment.from};
    insert(&spoints, ips);
    spoints.emplace_back(segment.to);

    std::vector<Line> slines;
    for (std::size_t i = 0; i < (spoints.size() - 1); i += 2) {
      const auto subSegment = Line{spoints[i], spoints[i + 1]};
      if (this->midPointInPolygon(polygon, subSegment)) {
        slines.emplace_back(subSegment);
      }
    }
    return slines;
  } else if (this->midPointInPolygon(polygon, segment)) {
    return {segment};
  } else {
    return {};
  }
}
