#include "rough/fillers/filler.h"

#include <cassert>

#include "rough/core.h"
#include "rough/fillers/filler-interface.h"
#include "rough/fillers/hachure-filler.h"
#include "rough/fillers/zigzag-filler.h"
#include "rough/fillers/hatch-filler.h"
#include "rough/fillers/dot-filler.h"
#include "rough/fillers/dashed-filler.h"
#include "rough/fillers/zigzag-line-filler.h"

// original code caches the fillers in a static array, but this is bad
// so we currently don't cache them
// todo(Gustav): look into this

std::unique_ptr<PatternFiller> getFiller(const Options& o, RenderHelper* helper) {
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
