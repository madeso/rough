#include "hachure-filler.h"

struct ZigZagFiller : HachureFiller {
  ZigZagFiller(RenderHelper* rh);
  OpSet fillPolygon(const std::vector<Point>& points, ResolvedOptions& o) override;
};
