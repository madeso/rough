#pragma once

#include "filler-interface.h"
#include "core.h"
#include "geometry.h"
#include "scan-line-hachure.h"

struct DashedFiller: PatternFiller {
  RenderHelper* helper;

  DashedFiller(RenderHelper* helper);
  OpSet fillPolygon(const std::vector<Point>& points, ResolvedOptions& o) override;
  std::vector<Op> dashedLine(const std::vector<Line>& lines, ResolvedOptions& o);
};
