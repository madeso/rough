#pragma once

#include "core.h"
#include "geometry.h"
#include "fillers/filler.h"
#include "fillers/filler-interface.h"
#include "math.h"


struct EllipseParams {
  double rx;
  double ry;
  double increment;
};

struct EllipseResult {
  OpSet opset;
  std::vector<Point> estimatedPoints;
};

OpSet line(double x1, double y1, double x2, double y2, Options& o);
OpSet linearPath(const std::vector<Point>& points, bool close, Options& o);
OpSet polygon(const std::vector<Point>& points, Options& o);
OpSet rectangle(double x, double y, double width, double height, Options& o);
OpSet curve(const std::vector<Point>& points, const Options& o);
OpSet ellipse(double x, double y, double width, double height, const Options& o);
EllipseParams generateEllipseParams(double width, double height, const Options& o);
EllipseResult ellipseWithParams(double x, double y, const Options& o, const EllipseParams& ellipseParams);
OpSet arc(double x, double y, double width, double height, double start, double stop, bool closed, bool roughClosure, const Options& o);

#if 0
// parsePath, normalize, absolutize
#include "path-data-parser.h"
OpSet svgPath(string path, const Options& o);
#endif

// Fills

OpSet solidFillPolygon(const std::vector<Point>& points, const Options& o);
OpSet patternFillPolygon(const std::vector<Point>& points, Options& o);
OpSet patternFillArc(double x, double y, double width, double height, double start, double stop, const Options& o);
double randOffset(double x, const Options& o);
double randOffsetWithRange(double min, double max, const Options& o);
std::vector<Op> doubleLineFillOps(double x1, double y1, double x2, double y2, Options& o);
