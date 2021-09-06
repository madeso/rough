#pragma once

#include <optional>

#include "rough/core.h"
#include "rough/geometry.h"
#include "rough/renderer.h"
#include <cmath>

struct RoughGenerator
{
  static double newSeed();

  Drawable _d(ShapeType shape, std::vector<OpSet> sets, Options options);

  Drawable line(double x1, double y1, double x2, double y2, const Options& options);
  Drawable rectangle(double x, double y, double width, double height, const Options& options);
  Drawable ellipse(double x, double y, double width, double height, const Options& options);
  Drawable circle(double x, double y, double diameter, const Options& options);
  Drawable linearPath(std::vector<Point> points, const Options& options);
  Drawable arc(double x, double y, double width, double height, double start, double stop, bool closed /*= false*/, const Options& options);
  Drawable polygon(std::vector<Point> points, const Options& options);
};
