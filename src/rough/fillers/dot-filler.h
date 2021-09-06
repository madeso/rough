#pragma once

#include "rough/core.h"
#include "rough/geometry.h"

#include "rough/fillers/filler-interface.h"
#include "rough/fillers/scan-line-hachure.h"

struct DotFiller: PatternFiller {
  RenderHelper* helper;

  DotFiller(RenderHelper* helper);

  OpSet fillPolygon(const std::vector<Point>& points, Options& oo) override;
  OpSet dotsOnLines(const std::vector<Line>& lines, Options& o);
};
