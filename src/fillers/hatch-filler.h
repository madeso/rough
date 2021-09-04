#include "hachure-filler.h"
#include "core.h"
#include "geometry.h"

struct HatchFiller: HachureFiller {
  HatchFiller(RenderHelper* rh);
  OpSet fillPolygon(const std::vector<Point>& points, ResolvedOptions& o) override;
};
