#pragma once

#include "filler-interface.h"
#include "core.h"
#include "geometry.h"
#include "scan-line-hachure.h"

struct ZigZagLineFiller: PatternFiller {
  RenderHelper* helper;

  ZigZagLineFiller(RenderHelper* helper);

  OpSet fillPolygon(const std::vector<Point>& points, const ResolvedOptions& oo) override;

  std::vector<Op> zigzagLines(const std::vector<Line>& lines, double zo, const ResolvedOptions& o);
};
