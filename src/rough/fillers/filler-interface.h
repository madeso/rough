#pragma once

#include "rough/core.h"
#include "rough/geometry.h"

struct PatternFiller {
    virtual ~PatternFiller() = default;
  virtual OpSet fillPolygon(const std::vector<Point>& points, Options& o) = 0;
};

struct RenderHelper {
    virtual ~RenderHelper() = default;

  virtual double randOffset(double x, const Options& o) = 0;
  virtual double randOffsetWithRange(double min, double max, const Options& o) = 0;
  virtual OpSet ellipse(double x, double y, double width, double height, const Options& o) = 0;
  virtual std::vector<Op> doubleLineOps(double x1, double y1, double x2, double y2, Options& o) = 0;
  
  std::vector<Op> doubleLineOps(const Point& p1, const Point& p2, Options& o);
  std::vector<Op> doubleLineOps(const Line& line, Options& o);
};
