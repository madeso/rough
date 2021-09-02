#pragma once

#include "core.h"
#include "geometry.h"

struct PatternFiller {
    virtual ~PatternFiller() = default;
  virtual OpSet fillPolygon(const std::vector<Point>& points, const ResolvedOptions& o) = 0;
};

struct RenderHelper {
    virtual ~RenderHelper() = default;

  virtual double randOffset(double x, const ResolvedOptions& o) = 0;
  virtual double randOffsetWithRange(double min, double max, const ResolvedOptions& o) = 0;
  virtual OpSet ellipse(double x, double y, double width, double height, const ResolvedOptions& o) = 0;
  virtual std::vector<Op> doubleLineOps(double x1, double y1, double x2, double y2, const ResolvedOptions& o) = 0;
};
