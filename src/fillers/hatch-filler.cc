#include "hatch-filler.h"

#include "vector-utils.h"

OpSet HatchFiller::fillPolygon(const std::vector<Point>& points, const ResolvedOptions& o) {
  auto set = this->_fillPolygon(points, o);
  auto o2 = o; o2.hachureAngle += 90;
  const auto set2 = this->_fillPolygon(points, o2);
  insert(&set.ops, set2.ops);
  return set;
}
