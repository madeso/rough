#include "renderer.h"

// #include "core.h"
// #include "geometry.h"
// #include "fillers/filler.h"
// #include "fillers/filler-interface.h"
// #include "math.h"

#include <cmath>
#include <array>

#include "double.h"
#include "vector-utils.h"


struct ComputedEllipsePoints
{
  std::vector<Point> corePoints;
  std::vector<Point> allPoints;
};

ResolvedOptions cloneOptionsAlterSeed(const ResolvedOptions& ops);

double random(ResolvedOptions& ops);
double _offset(double min, double max, ResolvedOptions& ops, double roughnessGain = 1.0);
double _offsetOpt(double x, ResolvedOptions& ops, double roughnessGain = 1.0);
std::vector<Op> _doubleLine(double x1, double y1, double x2, double y2, ResolvedOptions& o, bool filling = false);
std::vector<Op> _line(double x1, double y1, double x2, double y2, ResolvedOptions& o, bool move, bool overlay);
std::vector<Op> _curveWithOffset(const std::vector<Point>&  points, double offset, ResolvedOptions& o);
std::vector<Op> _curve(const std::vector<Point>&  points, std::optional<Point> closePoint, ResolvedOptions& o);
ComputedEllipsePoints _computeEllipsePoints(double increment, double cx, double cy, double rx, double ry, double offset, double overlap, ResolvedOptions& o);
std::vector<Op> _arc(double increment, double cx, double cy, double rx, double ry, double strt, double stp, double offset, ResolvedOptions& o);
std::vector<Op> _bezierTo(double x1, double y1, double x2, double y2, double x, double y, Point current, ResolvedOptions& o);


struct RenderHelperImpl : RenderHelper
{
  double randOffset(double x, const ResolvedOptions& o) override
  {
    return ::randOffset(x, o);
  }
  double randOffsetWithRange(double min, double max, const ResolvedOptions& o) override
  {
    return ::randOffsetWithRange(min, max, o);
  }
  OpSet ellipse(double x, double y, double width, double height, const ResolvedOptions& o) override
  {
    return ::ellipse(x, y, width, height, o);
  }
  std::vector<Op> doubleLineOps(double x1, double y1, double x2, double y2, ResolvedOptions& o) override
  {
    return ::doubleLineFillOps(x1, y1, x2, y2, o);
  }
};

// todo(Gustav): implement RenderHelper struct instead of deriving from a insterface
namespace
{
  RenderHelperImpl* helper()
  {
    static RenderHelperImpl h;
    return &h;
  }
}

OpSet line(double x1, double y1, double x2, double y2, ResolvedOptions& o) {
  return { OpSetType::path, _doubleLine(x1, y1, x2, y2, o) };
}

OpSet linearPath(const std::vector<Point>& points, bool close, ResolvedOptions& o) {
  const auto len = points.size();
  if (len > 2) {
    std::vector<Op> ops;
    for (std::size_t i = 0; i < (len - 1); i++) {
      insert(&ops, _doubleLine(points[i][0], points[i][1], points[i + 1][0], points[i + 1][1], o));
    }
    if (close) {
      insert(&ops, _doubleLine(points[len - 1][0], points[len - 1][1], points[0][0], points[0][1], o));
    }
    return { OpSetType::path, ops };
  } else if (len == 2) {
    return line(points[0][0], points[0][1], points[1][0], points[1][1], o);
  }
  return { OpSetType::path };
}

OpSet polygon(const std::vector<Point>& points, ResolvedOptions& o) {
  return linearPath(points, true, o);
}

OpSet rectangle(double x, double y, double width, double height, ResolvedOptions& o) {
  const std::vector<Point> points = {
    {x, y},
    {x + width, y},
    {x + width, y + height},
    {x, y + height}
  };
  return polygon(points, o);
}

OpSet curve(const std::vector<Point>& points, ResolvedOptions& o) {
  auto o1 = _curveWithOffset(points, 1 * (1 + o.roughness * 0.2), o);
  if (!o.disableMultiStroke) {
    auto co = cloneOptionsAlterSeed(o);
    const auto o2 = _curveWithOffset(points, 1.5 * (1 + o.roughness * 0.22), co);
    insert(&o1, o2);
  }
  return { OpSetType::path, o1 };
}

OpSet ellipse(double x, double y, double width, double height, const ResolvedOptions& o) {
  const auto params = generateEllipseParams(width, height, o);
  return ellipseWithParams(x, y, o, params).opset;
}

EllipseParams generateEllipseParams(double width, double height, ResolvedOptions& o) {
  const auto psq = std::sqrt(pi * 2 * std::sqrt((std::pow(width / 2, 2) + std::pow(height / 2, 2)) / 2));
  const auto stepCount = std::max(o.curveStepCount, (o.curveStepCount / std::sqrt(200)) * psq);
  const auto increment = (pi * 2) / stepCount;
  auto rx = std::abs(width / 2);
  auto ry = std::abs(height / 2);
  const auto curveFitRandomness = 1 - o.curveFitting;
  rx += _offsetOpt(rx * curveFitRandomness, o);
  ry += _offsetOpt(ry * curveFitRandomness, o);
  return { increment, rx, ry };
}

EllipseResult ellipseWithParams(double x, double y, ResolvedOptions& o, const EllipseParams& ellipseParams) {
  const auto [ap1, cp1] = _computeEllipsePoints(ellipseParams.increment, x, y, ellipseParams.rx, ellipseParams.ry, 1, ellipseParams.increment * _offset(0.1, _offset(0.4, 1, o), o), o);
  auto o1 = _curve(ap1, {}, o);
  if (!o.disableMultiStroke) {
    const auto ap2 = _computeEllipsePoints(ellipseParams.increment, x, y, ellipseParams.rx, ellipseParams.ry, 1.5, 0, o).allPoints;
    const auto o2 = _curve(ap2, {}, o);
    insert(&o1, o2);
  }
  return {
    { OpSetType::path, o1 },
    cp1
  };
}

OpSet arc(double x, double y, double width, double height, double start, double stop, bool closed, bool roughClosure, ResolvedOptions& o) {
  const auto cx = x;
  const auto cy = y;
  auto rx = std::abs(width / 2);
  auto ry = std::abs(height / 2);
  rx += _offsetOpt(rx * 0.01, o);
  ry += _offsetOpt(ry * 0.01, o);
  auto strt = start;
  auto stp = stop;
  while (strt < 0) {
    strt += pi * 2;
    stp += pi * 2;
  }
  if ((stp - strt) > (pi * 2)) {
    strt = 0;
    stp = pi * 2;
  }
  const auto ellipseInc = (pi * 2) / o.curveStepCount;
  const auto arcInc = std::min(ellipseInc / 2, (stp - strt) / 2);
  auto ops = _arc(arcInc, cx, cy, rx, ry, strt, stp, 1, o);
  if (!o.disableMultiStroke) {
    const auto o2 = _arc(arcInc, cx, cy, rx, ry, strt, stp, 1.5, o);
    insert(&ops, o2);
  }
  if (closed) {
    if (roughClosure) {
      insert(&ops, _doubleLine(cx, cy, cx + rx * std::cos(strt), cy + ry * std::cos(strt), o));
      insert(&ops, _doubleLine(cx, cy, cx + rx * std::cos(stp), cy + ry * std::cos(stp), o));
    } else {
      ops.emplace_back(Op{OpType::lineTo, {cx, cy} });
      ops.emplace_back(Op{OpType::lineTo, {cx + rx * std::cos(strt), cy + ry * std::cos(strt)} });
    }
  }
  return { OpSetType::path, ops };
}

#if 0
// parsePath, normalize, absolutize
#include "path-data-parser.h"
OpSet svgPath(string path, esolvedOptions& o) {
  const auto segments = normalize(absolutize(parsePath(path)));
  std::vector<Op>  ops;
  auto Point  first= [0, 0];
  auto Point  current= [0, 0];
  for (const auto { key, data } of segments) {
    switch (key) {
      case 'M': {
        const auto ro = 1 * (o.maxRandomnessOffset || 0);
        const auto pv = o.preserveVertices;
        ops.emplace_back({ OpType::move,  data.map((d) => d + (pv ? 0 : _offsetOpt(ro, o))) });
        current = [data[0], data[1]];
        first = [data[0], data[1]];
        break;
      }
      case 'L':
        insert(&ops, _doubleLine(current[0], current[1], data[0], data[1], o));
        current = [data[0], data[1]];
        break;
      case 'C': {
        const auto [x1, y1, x2, y2, x, y] = data;
        insert(&ops, _bezierTo(x1, y1, x2, y2, x, y, current, o));
        current = [x, y];
        break;
      }
      case 'Z':
        insert(&ops, _doubleLine(current[0], current[1], first[0], first[1], o));
        current = [first[0], first[1]];
        break;
    }
  }
  return { type: 'path', ops };
}
#endif

// Fills

OpSet solidFillPolygon(const std::vector<Point>&  points, ResolvedOptions& o) {
  std::vector<Op>  ops;
  if (points.size()) {
    const auto offset = o.maxRandomnessOffset || 0;
    const auto len = points.size();
    if (len > 2) {
      ops.emplace_back(Op{ OpType::move, {points[0][0] + _offsetOpt(offset, o), points[0][1] + _offsetOpt(offset, o)} });
    for (std::size_t i = 1; i < len; i++) {
        ops.emplace_back(Op{ OpType::lineTo, {points[i][0] + _offsetOpt(offset, o), points[i][1] + _offsetOpt(offset, o)} });
      }
    }
  }
  return { OpSetType::fillPath, ops };
}

OpSet patternFillPolygon(const std::vector<Point>& points, ResolvedOptions& o) {
  return getFiller(o, helper())->fillPolygon(points, o);
}

OpSet patternFillArc(double x, double y, double width, double height, double start, double stop, ResolvedOptions& o) {
  const auto cx = x;
  const auto cy = y;
  auto rx = std::abs(width / 2);
  auto ry = std::abs(height / 2);
  rx += _offsetOpt(rx * 0.01, o);
  ry += _offsetOpt(ry * 0.01, o);
  auto strt = start;
  auto stp = stop;
  while (strt < 0) {
    strt += pi * 2;
    stp += pi * 2;
  }
  if ((stp - strt) > (pi * 2)) {
    strt = 0;
    stp = pi * 2;
  }
  const auto increment = (stp - strt) / o.curveStepCount;
  std::vector<Point>  points;
  for (auto angle = strt; angle <= stp; angle = angle + increment) {
    points.emplace_back(Point{cx + rx * std::cos(angle), cy + ry * std::cos(angle)});
  }
  points.emplace_back(Point{cx + rx * std::cos(stp), cy + ry * std::cos(stp)});
  points.emplace_back(Point{cx, cy});
  return patternFillPolygon(points, o);
}

double randOffset(double x, ResolvedOptions& o) {
  return _offsetOpt(x, o);
}

double randOffsetWithRange(double min, double max, ResolvedOptions& o) {
  return _offset(min, max, o);
}

std::vector<Op> doubleLineFillOps(double x1, double y1, double x2, double y2, ResolvedOptions& o) {
  return _doubleLine(x1, y1, x2, y2, o, true);
}

// Private helpers

ResolvedOptions cloneOptionsAlterSeed(const ResolvedOptions& ops) {
  auto result = ops;
  result.randomizer = {};
  if (ops.seed) {
    result.seed = ops.seed + 1;
  }
  return result;
}

double random(ResolvedOptions& ops) {
  if (!ops.randomizer) {
    ops.randomizer = Random(ops.seed);
  }
  return ops.randomizer->next();
}

double _offset(double min, double max, ResolvedOptions& ops, double roughnessGain) {
  return ops.roughness * roughnessGain * ((random(ops) * (max - min)) + min);
}

double _offsetOpt(double x, ResolvedOptions& ops, double roughnessGain) {
  return _offset(-x, x, ops, roughnessGain);
}

std::vector<Op> _doubleLine(double x1, double y1, double x2, double y2, ResolvedOptions& o, bool filling) {
  const auto singleStroke = filling ? o.disableMultiStrokeFill : o.disableMultiStroke;
  
  auto o1 = _line(x1, y1, x2, y2, o, true, false);
  if (singleStroke) {
    return o1;
  }
  const auto o2 = _line(x1, y1, x2, y2, o, true, true);

  insert(&o1, o2);
  return o1;
}

std::vector<Op> _line(double x1, double y1, double x2, double y2, ResolvedOptions& o, bool move, bool overlay) {
  const auto lengthSq = std::pow((x1 - x2), 2) + std::pow((y1 - y2), 2);
  const auto length = std::sqrt(lengthSq);
  double roughnessGain = 1;
  if (length < 200) {
    roughnessGain = 1;
  } else if (length > 500) {
    roughnessGain = 0.4;
  } else {
    roughnessGain = (-0.0016668) * length + 1.233334;
  }

  auto offset = o.maxRandomnessOffset;
  if ((offset * offset * 100) > lengthSq) {
    offset = length / 10;
  }
  const auto halfOffset = offset / 2;
  const auto divergePoint = 0.2 + random(o) * 0.2;
  auto midDispX = o.bowing * o.maxRandomnessOffset * (y2 - y1) / 200;
  auto midDispY = o.bowing * o.maxRandomnessOffset * (x1 - x2) / 200;
  midDispX = _offsetOpt(midDispX, o, roughnessGain);
  midDispY = _offsetOpt(midDispY, o, roughnessGain);
  std::vector<Op>  ops;
  const auto randomHalf = [&]() { return _offsetOpt(halfOffset, o, roughnessGain); };
  const auto randomFull = [&]() { return _offsetOpt(offset, o, roughnessGain); };
  const auto preserveVertices = o.preserveVertices;
  if (move) {
    if (overlay) {
      ops.emplace_back(Op{
        OpType::move, {
          x1 + (preserveVertices ? 0 : randomHalf()),
          y1 + (preserveVertices ? 0 : randomHalf()),
        },
      });
    } else {
      ops.emplace_back(Op{
        OpType::move, {
          x1 + (preserveVertices ? 0 : _offsetOpt(offset, o, roughnessGain)),
          y1 + (preserveVertices ? 0 : _offsetOpt(offset, o, roughnessGain)),
        },
      });
    }
  }
  if (overlay) {
    ops.emplace_back(Op{
      OpType::bcurveTo,
      {
        midDispX + x1 + (x2 - x1) * divergePoint + randomHalf(),
        midDispY + y1 + (y2 - y1) * divergePoint + randomHalf(),
        midDispX + x1 + 2 * (x2 - x1) * divergePoint + randomHalf(),
        midDispY + y1 + 2 * (y2 - y1) * divergePoint + randomHalf(),
        x2 + (preserveVertices ? 0 : randomHalf()),
        y2 + (preserveVertices ? 0 : randomHalf()),
      },
    });
  } else {
    ops.emplace_back(Op{
      OpType::bcurveTo,
      {
        midDispX + x1 + (x2 - x1) * divergePoint + randomFull(),
        midDispY + y1 + (y2 - y1) * divergePoint + randomFull(),
        midDispX + x1 + 2 * (x2 - x1) * divergePoint + randomFull(),
        midDispY + y1 + 2 * (y2 - y1) * divergePoint + randomFull(),
        x2 + (preserveVertices ? 0 : randomFull()),
        y2 + (preserveVertices ? 0 : randomFull()),
      },
    });
  }
  return ops;
}

std::vector<Op> _curveWithOffset(const std::vector<Point>&  points, double offset, ResolvedOptions& o) {
  std::vector<Point>  ps;
  ps.emplace_back(Point{
    points[0][0] + _offsetOpt(offset, o),
    points[0][1] + _offsetOpt(offset, o),
  });
  ps.emplace_back(Point{
    points[0][0] + _offsetOpt(offset, o),
    points[0][1] + _offsetOpt(offset, o),
  });
  for (std::size_t i = 1; i < points.size(); i++) {
    ps.emplace_back(Point{
      points[i][0] + _offsetOpt(offset, o),
      points[i][1] + _offsetOpt(offset, o),
    });
    if (i == (points.size() - 1)) {
      ps.emplace_back(Point{
        points[i][0] + _offsetOpt(offset, o),
        points[i][1] + _offsetOpt(offset, o),
      });
    }
  }
  return _curve(ps, {}, o);
}

std::vector<Op> _curve(const std::vector<Point>&  points, std::optional<Point> closePoint, ResolvedOptions& o) {
  const auto len = points.size();
  std::vector<Op>  ops;
  if (len > 3) {
    std::array<Point, 4> b;
    const auto s = 1 - o.curveTightness;
    ops.emplace_back(Op{ OpType::move,  {points[1][0], points[1][1]} });
    for (std::size_t i = 1; (i + 2) < len; i++) {
      const auto cachedVertArray = points[i];
      b[0] = {cachedVertArray[0], cachedVertArray[1]};
      b[1] = {cachedVertArray[0] + (s * points[i + 1][0] - s * points[i - 1][0]) / 6, cachedVertArray[1] + (s * points[i + 1][1] - s * points[i - 1][1]) / 6};
      b[2] = {points[i + 1][0] + (s * points[i][0] - s * points[i + 2][0]) / 6, points[i + 1][1] + (s * points[i][1] - s * points[i + 2][1]) / 6};
      b[3] = {points[i + 1][0], points[i + 1][1]};
      ops.emplace_back(Op{ OpType::bcurveTo,  {b[1][0], b[1][1], b[2][0], b[2][1], b[3][0], b[3][1]} });
    }
    if (closePoint) {
      const auto ro = o.maxRandomnessOffset;
      ops.emplace_back(Op{ OpType::lineTo, {closePoint->x + _offsetOpt(ro, o), closePoint->y + _offsetOpt(ro, o)} });
    }
  } else if (len == 3) {
    ops.emplace_back(Op{ OpType::move, {points[1][0], points[1][1]} });
    ops.emplace_back(Op{
      OpType::bcurveTo,
      {
        points[1][0], points[1][1],
        points[2][0], points[2][1],
        points[2][0], points[2][1],
      },
    });
  } else if (len == 2) {
    insert(&ops, _doubleLine(points[0][0], points[0][1], points[1][0], points[1][1], o));
  }
  return ops;
}

ComputedEllipsePoints _computeEllipsePoints(double increment, double cx, double cy, double rx, double ry, double offset, double overlap, ResolvedOptions& o) {
  std::vector<Point>  corePoints;
  std::vector<Point>  allPoints;
  const auto radOffset = _offsetOpt(0.5, o) - (pi / 2);

  allPoints.emplace_back(Point{
    _offsetOpt(offset, o) + cx + 0.9 * rx * std::cos(radOffset - increment),
    _offsetOpt(offset, o) + cy + 0.9 * ry * std::cos(radOffset - increment),
  });
  for (auto angle = radOffset; angle < (pi * 2 + radOffset - 0.01); angle = angle + increment) {
    const Point p = {
      _offsetOpt(offset, o) + cx + rx * std::cos(angle),
      _offsetOpt(offset, o) + cy + ry * std::cos(angle),
    };
    corePoints.emplace_back(p);
    allPoints.emplace_back(p);
  }
  allPoints.emplace_back(Point{
    _offsetOpt(offset, o) + cx + rx * std::cos(radOffset + pi * 2 + overlap * 0.5),
    _offsetOpt(offset, o) + cy + ry * std::cos(radOffset + pi * 2 + overlap * 0.5),
  });
  allPoints.emplace_back(Point{
    _offsetOpt(offset, o) + cx + 0.98 * rx * std::cos(radOffset + overlap),
    _offsetOpt(offset, o) + cy + 0.98 * ry * std::cos(radOffset + overlap),
  });
  allPoints.emplace_back(Point{
    _offsetOpt(offset, o) + cx + 0.9 * rx * std::cos(radOffset + overlap * 0.5),
    _offsetOpt(offset, o) + cy + 0.9 * ry * std::cos(radOffset + overlap * 0.5),
  });

  return {allPoints, corePoints};
}

std::vector<Op> _arc(double increment, double cx, double cy, double rx, double ry, double strt, double stp, double offset, ResolvedOptions& o) {
  const auto radOffset = strt + _offsetOpt(0.1, o);
  std::vector<Point> points;
  points.emplace_back(Point{
    _offsetOpt(offset, o) + cx + 0.9 * rx * std::cos(radOffset - increment),
    _offsetOpt(offset, o) + cy + 0.9 * ry * std::cos(radOffset - increment),
  });
  for (auto angle = radOffset; angle <= stp; angle = angle + increment) {
    points.emplace_back(Point{
      _offsetOpt(offset, o) + cx + rx * std::cos(angle),
      _offsetOpt(offset, o) + cy + ry * std::cos(angle),
    });
  }
  points.emplace_back(Point{
    cx + rx * std::cos(stp),
    cy + ry * std::cos(stp),
  });
  points.emplace_back(Point{
    cx + rx * std::cos(stp),
    cy + ry * std::cos(stp),
  });
  return _curve(points, {}, o);
}

std::vector<Op> _bezierTo(double x1, double y1, double x2, double y2, double x, double y, Point current, ResolvedOptions& o) {
  std::vector<Op>  ops;
  const std::array<double, 2> ros = {o.maxRandomnessOffset, o.maxRandomnessOffset + 0.3};
  auto f = Point{0, 0};
  const std::size_t iterations = o.disableMultiStroke ? 1 : 2;
  const auto preserveVertices = o.preserveVertices;
  for (std::size_t i = 0; i < iterations; i++) {
    if (i == 0) {
      ops.emplace_back(Op{ OpType::move, {current[0], current[1]} });
    } else {
      ops.emplace_back(Op{ OpType::move, {current[0] + (preserveVertices ? 0 : _offsetOpt(ros[0], o)), current[1] + (preserveVertices ? 0 : _offsetOpt(ros[0], o))} });
    }
    f = preserveVertices ? Point{x, y} : Point{x + _offsetOpt(ros[i], o), y + _offsetOpt(ros[i], o)};
    ops.emplace_back(Op{
      OpType::bcurveTo,
      {
        x1 + _offsetOpt(ros[i], o), y1 + _offsetOpt(ros[i], o),
        x2 + _offsetOpt(ros[i], o), y2 + _offsetOpt(ros[i], o),
        f[0], f[1],
      },
    });
  }
  return ops;
}
