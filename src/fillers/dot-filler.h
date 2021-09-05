#pragma once

#include "filler-interface.h"
#include "core.h"
#include "geometry.h"
#include "scan-line-hachure.h"

struct DotFiller: PatternFiller {
  RenderHelper* helper;

  DotFiller(RenderHelper* helper);

  OpSet fillPolygon(const std::vector<Point>& points, Options& oo) override;
  OpSet dotsOnLines(const std::vector<Line>& lines, Options& o);
};
