#include "zigzag-filler.h"

ZigZagFiller::ZigZagFiller(RenderHelper* rh)
  : HachureFiller(rh)
{
}

OpSet ZigZagFiller::fillPolygon(const std::vector<Point>& points, const ResolvedOptions& o) {
  return this->_fillPolygon(points, o, true);
}
