#include "rough/fillers/hachure-filler.h"

#include "rough/core.h"
#include "rough/geometry.h"

struct HatchFiller: HachureFiller {
  HatchFiller(RenderHelper* rh);
  OpSet fillPolygon(const std::vector<Point>& points, Options& o) override;
};
