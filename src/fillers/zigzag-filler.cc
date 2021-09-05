#include "zigzag-filler.h"

ZigZagFiller::ZigZagFiller(RenderHelper* rh)
  : HachureFiller(rh)
{
}

OpSet ZigZagFiller::fillPolygon(const std::vector<Point>& points, Options& o) {
  return this->_fillPolygon(points, o, true);
}
