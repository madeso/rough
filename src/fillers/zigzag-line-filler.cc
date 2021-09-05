#include "zigzag-line-filler.h"

#include <cmath>

#include "vector-utils.h"
#include "double.h"

ZigZagLineFiller::ZigZagLineFiller(RenderHelper* helper) {
  this->helper = helper;
}

OpSet ZigZagLineFiller::fillPolygon(const std::vector<Point>& points, Options& oo) {
  auto o = oo;
  const auto gap = o.hachureGap < 0 ? (o.strokeWidth * 4) : o.hachureGap;
  const auto zo = o.zigzagOffset < 0 ? gap : o.zigzagOffset;
  o.hachureGap = gap + zo;
  const auto lines = polygonHachureLines(points, o);
  return { OpSetType::fillSketch, this->zigzagLines(lines, zo, o) };
}

std::vector<Op> ZigZagLineFiller::zigzagLines(const std::vector<Line>& lines, double zo, Options& o) {
  std::vector<Op>  ops;
  for(const auto& line: lines){
    const auto length = lineLength(line);
    const auto countf = std::round(length / (2 * zo));
    const auto count = static_cast<std::size_t>(countf);
    auto p1 = line.from;
    auto p2 = line.to;
    if (p1.x > p2.x) {
      p1 = line.to;
      p2 = line.from;
    }
    const auto alpha = std::atan((p2.y - p1.y) / (p2.x - p1.x));
    for (std::size_t i = 0; i < count; i++) {
      const auto lstart = i * 2 * zo;
      const auto lend = (i + 1) * 2 * zo;
      const auto dz = std::sqrt(2 * std::pow(zo, 2));
      const auto start = Point{p1.x + (lstart * std::cos(alpha)), p1.y + lstart * std::sin(alpha)};
      const auto end = Point{p1.x + (lend * std::cos(alpha)), p1.y + (lend * std::sin(alpha))};
      const auto middle = Point{start.x + dz * std::cos(alpha + pi / 4), start.y + dz * std::sin(alpha + pi / 4)};
      insert(&ops, this->helper->doubleLineOps(start.x, start.y, middle.x, middle.y, o));
      insert(&ops, this->helper->doubleLineOps(middle.x, middle.y, end.x, end.y, o));
    }
  }
  return ops;
}
