#include "dashed-filler.h"

#include <cmath>

#include "rough/vector-utils.h"

DashedFiller::DashedFiller(RenderHelper* helper) {
  this->helper = helper;
}

OpSet DashedFiller::fillPolygon(const std::vector<Point>& points, Options& o) {
  const auto lines = polygonHachureLines(points, o);
  return OpSet{ OpSetType::fillSketch, this->dashedLine(lines, o) };
}

std::vector<Op> DashedFiller::dashedLine(const std::vector<Line>& lines, Options& o) {
  const auto offset = o.dashOffset < 0 ? (o.hachureGap < 0 ? (o.strokeWidth * 4) : o.hachureGap) : o.dashOffset;
  const auto gap = o.dashGap < 0 ? (o.hachureGap < 0 ? (o.strokeWidth * 4) : o.hachureGap) : o.dashGap;
  std::vector<Op> ops;
  for(auto& line: lines) {
    const auto length = lineLength(line);
    const auto countf = std::floor(length / (offset + gap));
    const auto count = static_cast<std::size_t>(countf);
    const auto startOffset = (length + gap - (count * (offset + gap))) / 2;
    auto p1 = line.from;
    auto p2 = line.to;
    if (p1.x > p2.x) {
      p1 = line.to;
      p2 = line.from;
    }
    const auto alpha = std::atan((p2.y - p1.y) / (p2.x - p1.x));
    for (std::size_t i = 0; i < count; i++) {
      const auto lstart = i * (offset + gap);
      const auto lend = lstart + offset;
      const Point start =
      {
        p1.x + (lstart * std::cos(alpha)) + (startOffset * std::cos(alpha)),
        p1.y + (lstart * std::sin(alpha)) + (startOffset * std::sin(alpha))
      };
      const Point end =
      {
        p1.x + (lend * std::cos(alpha)) + (startOffset * std::cos(alpha)),
        p1.y + (lend * std::sin(alpha)) + (startOffset * std::sin(alpha))
      };
      insert(&ops, this->helper->doubleLineOps(start, end, o));
    }
  }
  return ops;
}
