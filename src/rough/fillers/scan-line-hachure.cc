#include "rough/fillers/scan-line-hachure.h"

#include "rough/double.h"
#include "rough/vector-utils.h"

struct EdgeEntry {
  double ymin;
  double ymax;
  double x;
  double islope;
};

struct ActiveEdgeEntry {
  double s;
  EdgeEntry edge;
};


// double round(double x) {
//  return std::floor(x + 0.5);
// }

std::vector<Line> straightHachureLines(const std::vector<Point>& points, const Options& o) {
  auto vertices = points;
  if (vertices[0] != vertices[vertices.size() - 1]) {
    vertices.push_back(vertices[0]);
  }
  std::vector<Line> lines;

  if (vertices.size() > 2) {
    auto gap = o.hachureGap;
    if (gap < 0) {
      gap = o.strokeWidth * 4;
    }
    gap = std::max(gap, 0.1);

    // Create sorted edges table
    std::vector<EdgeEntry> edges;
    for (size_t i = 0; i < vertices.size() - 1; i++) {
      const auto p1 = vertices[i];
      const auto p2 = vertices[i + 1];
      if (!isSame(p1.y, p2.y)) {
        const auto ymin = std::min(p1.y, p2.y);
        edges.push_back({
          ymin,
          std::max(p1.y, p2.y),
          isSame(ymin, p1.y) ? p1.x : p2.x,
          (p2.x - p1.x) / (p2.y - p1.y)
        });
      }
    }
    sort(edges, [](const EdgeEntry& e1, const EdgeEntry& e2) -> double {
      if (e1.ymin < e2.ymin) {
        return -1;
      }
      if (e1.ymin > e2.ymin) {
        return 1;
      }
      if (e1.x < e2.x) {
        return -1;
      }
      if (e1.x > e2.x) {
        return 1;
      }
      if (isSame(e1.ymax, e2.ymax)) {
        return 0;
      }
      return (e1.ymax - e2.ymax); // / Math.abs((e1.ymax - e2.ymax));
    });
    if (edges.empty()) {
      return lines;
    }

    // Start scanning
    std::vector<ActiveEdgeEntry> activeEdges;
    auto y = edges[0].ymin;
    while (activeEdges.size() || edges.size()) {
      if (edges.size()) {
        std::size_t ix = 0;
        for (const auto& edge: edges) {
          if (edge.ymin > y) {
            break;
          }
          ix += 1;
        }
        const auto start = edges.begin();
        const auto end = edges.begin() + ix;
        for(auto edge=start; edge != end; edge++) {
          activeEdges.push_back({ y, *edge });
        }
        edges.erase(start, end);
      }
      activeEdges = filter(activeEdges, [&](const ActiveEdgeEntry& ae){
        if (ae.edge.ymax <= y) {
          return false;
        }
        return true;
      });
      sort(activeEdges, [](const ActiveEdgeEntry& ae1, const ActiveEdgeEntry& ae2){
        if (isSame(ae1.edge.x, ae2.edge.x)) {
          return 0.0;
        }
        return (ae1.edge.x - ae2.edge.x); // / Math.abs((ae1.edge.x - ae2.edge.x));
      });

      // fill between the edges
      if (activeEdges.size() > 1) {
        for (std::size_t i = 0; i < activeEdges.size(); i += 2) {
          const auto nexti = i + 1;
          if (nexti >= activeEdges.size()) {
            break;
          }
          const auto ce = activeEdges[i].edge;
          const auto ne = activeEdges[nexti].edge;
          lines.emplace_back(Line{
            Point{round(ce.x), y},
            Point{round(ne.x), y}
          });
        }
      }

      y += gap;
      for(auto& ae: activeEdges){
        ae.edge.x = ae.edge.x + (gap * ae.edge.islope);
      }
    }
  }
  return lines;
}



std::vector<Line> polygonHachureLines(const std::vector<Point>& ppoints, const Options& o) {
    auto points = ppoints;
  const auto rotationCenter = Point{0, 0};
  const auto angle = round(o.hachureAngle + 90);
  if (!isZero(angle)) {
    rotatePoints(&points, rotationCenter, angle);
  }
  auto lines = straightHachureLines(points, o);
  if (!isZero(angle)) {
    rotatePoints(&points, rotationCenter, -angle);
    rotateLines(&lines, rotationCenter, -angle);
  }
  return lines;
}
