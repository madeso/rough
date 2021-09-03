#pragma once

#include <vector>

#include "filler-interface.h"
#include "core.h"
#include "geometry.h"
#include "scan-line-hachure.h"

struct HachureFiller : PatternFiller {
  RenderHelper* helper;

  explicit HachureFiller(RenderHelper* helper);

  OpSet fillPolygon(const std::vector<Point>& points, const ResolvedOptions& o);
  OpSet _fillPolygon(const std::vector<Point>& points, const ResolvedOptions& o, bool connectEnds= false);
  std::vector<Op> renderLines(const std::vector<Line>& lines, const ResolvedOptions& o);
  std::vector<Line> connectingLines(const std::vector<Point>& polygon, const std::vector<Line>& lines);
  bool midPointInPolygon(const std::vector<Point>& polygon, const Line& segment);
  std::vector<Line> splitOnIntersections(const std::vector<Point>& polygon, const Line& segment);
};
