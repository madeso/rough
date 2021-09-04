#include "dot-filler.h"

#include <cmath>
#include "vector-utils.h"

DotFiller::DotFiller(RenderHelper* helper) {
  this->helper = helper;
}

OpSet DotFiller::fillPolygon(const std::vector<Point>& points, ResolvedOptions& oo) {
  auto o = oo;
  o.curveStepCount = 4;
  o.hachureAngle = 0;
  o.roughness = 1;
  const auto lines = polygonHachureLines(points, o);
  return this->dotsOnLines(lines, o);
}

OpSet DotFiller::dotsOnLines(const std::vector<Line>& lines, ResolvedOptions& o) {
  std::vector<Op> ops;
  auto gap = o.hachureGap;
  if (gap < 0) {
    gap = o.strokeWidth * 4;
  }
  gap = std::max(gap, 0.1);
  auto fweight = o.fillWeight;
  if (fweight < 0) {
    fweight = o.strokeWidth / 2;
  }
  const auto ro = gap / 4;
  for (const auto& line: lines) {
    const auto length = lineLength(line);
    const auto dl = length / gap;
    const auto countf = std::ceil(dl) - 1;
    const auto count = static_cast<std::size_t>(countf);
    const auto offset = length - (count * gap);
    const auto x = ((line.from.x + line.to.x) / 2) - (gap / 4);
    const auto minY = std::min(line.from.y, line.to.y);

    for (std::size_t i = 0; i < count; i++) {
      const auto y = minY + offset + (i * gap);
      const auto cx = this->helper->randOffsetWithRange(x - ro, x + ro, o);
      const auto cy = this->helper->randOffsetWithRange(y - ro, y + ro, o);
      const auto el = this->helper->ellipse(cx, cy, fweight, fweight, o);
      insert(&ops, el.ops);
    }
  }
  return { OpSetType::fillSketch, ops };
}
