#include "rough/fillers/hachure-filler.h"

struct ZigZagFiller : HachureFiller {
  ZigZagFiller(RenderHelper* rh);
  OpSet fillPolygon(const std::vector<Point>& points, const Options& o) override;
};
