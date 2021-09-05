#pragma once

#include <vector>

#include "filler-interface.h"
#include "core.h"
#include "geometry.h"
#include "scan-line-hachure.h"

struct HachureFiller : PatternFiller {
  RenderHelper* helper;

  explicit HachureFiller(RenderHelper* helper);

  virtual OpSet fillPolygon(const std::vector<Point>& points, Options& o);
  OpSet _fillPolygon(const std::vector<Point>& points, Options& o, bool connectEnds= false);
  std::vector<Op> renderLines(const std::vector<Line>& lines, Options& o);
  std::vector<Line> connectingLines(const std::vector<Point>& polygon, const std::vector<Line>& lines);
  bool midPointInPolygon(const std::vector<Point>& polygon, const Line& segment);
  std::vector<Line> splitOnIntersections(const std::vector<Point>& polygon, const Line& segment);
};
