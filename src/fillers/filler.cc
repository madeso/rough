#include "filler.h"

#include <cassert>

#include "core.h"
#include "filler-interface.h"
#include "hachure-filler.h"
#include "zigzag-filler.h"
#include "hatch-filler.h"
#include "dot-filler.h"
#include "dashed-filler.h"
#include "zigzag-line-filler.h"

// orginal code caches the fillers in a static array, but this is bad
// so we currently don't cache them
// todo(Gustav): look into this

std::unique_ptr<PatternFiller> getFiller(const ResolvedOptions& o, RenderHelper* helper) {
  switch (o.fillStyle) {
    case FillStyle::zigzag:
      return std::make_unique<ZigZagFiller>(helper);
    case FillStyle::crossHatch:
      return std::make_unique<HatchFiller>(helper);
    case FillStyle::dots:
      return std::make_unique<DotFiller>(helper);
    case FillStyle::dashed:
      return std::make_unique<DashedFiller>(helper);
    case FillStyle::zigzagLine:
      return std::make_unique<ZigZagLineFiller>(helper);
    case FillStyle::hachure:
      return std::make_unique<HachureFiller>(helper);
    default:
      assert(false && "Unhandled fill style");
      return nullptr;
  }
}
