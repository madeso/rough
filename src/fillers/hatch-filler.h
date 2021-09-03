#include "hachure-filler.h"
#include "core.h"
#include "geometry.h"

struct HatchFiller: HachureFiller {
  OpSet fillPolygon(const std::vector<Point>& points, const ResolvedOptions& o) override;
};