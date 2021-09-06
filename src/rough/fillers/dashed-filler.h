#pragma once

#include "rough/core.h"
#include "rough/geometry.h"

#include "rough/fillers/filler-interface.h"
#include "rough/fillers/scan-line-hachure.h"

struct DashedFiller: PatternFiller {
  RenderHelper* helper;

  DashedFiller(RenderHelper* helper);
  OpSet fillPolygon(const std::vector<Point>& points, const Options& o) override;
  std::vector<Op> dashedLine(const std::vector<Line>& lines, const Options& o);
};
