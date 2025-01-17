#pragma once

#include <vector>

#include "rough/core.h"
#include "rough/geometry.h"

#include "rough/fillers/filler-interface.h"
#include "rough/fillers/scan-line-hachure.h"

struct HachureFiller : PatternFiller {
  RenderHelper* helper;

  explicit HachureFiller(RenderHelper* helper);

  virtual OpSet fillPolygon(const std::vector<Point>& points, const Options& o);
  OpSet _fillPolygon(const std::vector<Point>& points, const Options& o, bool connectEnds= false);
  std::vector<Op> renderLines(const std::vector<Line>& lines, const Options& o);
  std::vector<Line> connectingLines(const std::vector<Point>& polygon, const std::vector<Line>& lines);
  bool midPointInPolygon(const std::vector<Point>& polygon, const Line& segment);
  std::vector<Line> splitOnIntersections(const std::vector<Point>& polygon, const Line& segment);
};
