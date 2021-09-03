#include "zigzag-filler.h"

OpSet ZigZagFiller::fillPolygon(const std::vector<Point>& points, const ResolvedOptions& o) {
  return this->_fillPolygon(points, o, true);
}