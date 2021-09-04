#include "hatch-filler.h"

#include "vector-utils.h"

HatchFiller::HatchFiller(RenderHelper* rh)
  : HachureFiller(rh)
{
}

OpSet HatchFiller::fillPolygon(const std::vector<Point>& points, ResolvedOptions& o) {
  auto set = this->_fillPolygon(points, o);
  auto o2 = o; o2.hachureAngle += 90;
  const auto set2 = this->_fillPolygon(points, o2);
  insert(&set.ops, set2.ops);
  return set;
}
