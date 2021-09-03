#include "hachure-filler.h"

struct ZigZagFiller : HachureFiller {
  OpSet fillPolygon(const std::vector<Point>& points, const ResolvedOptions& o) override;
};
