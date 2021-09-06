#pragma once

#include "rough/core.h"
#include "rough/geometry.h"

#include "rough/fillers/filler-interface.h"
#include "rough/fillers/scan-line-hachure.h"


struct ZigZagLineFiller: PatternFiller {
  RenderHelper* helper;

  ZigZagLineFiller(RenderHelper* helper);

  OpSet fillPolygon(const std::vector<Point>& points, const Options& oo) override;

  std::vector<Op> zigzagLines(const std::vector<Line>& lines, double zo, const Options& o);
};
