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

OpSet line(double x1, double y1, double x2, double y2, ResolvedOptions& o);
OpSet linearPath(const std::vector<Point>& points, bool close, ResolvedOptions& o);
OpSet polygon(const std::vector<Point>& points, ResolvedOptions& o);
OpSet rectangle(double x, double y, double width, double height, ResolvedOptions& o);
OpSet curve(const std::vector<Point>& points, const ResolvedOptions& o);
OpSet ellipse(double x, double y, double width, double height, const ResolvedOptions& o);
EllipseParams generateEllipseParams(double width, double height, const ResolvedOptions& o);
EllipseResult ellipseWithParams(double x, double y, const ResolvedOptions& o, const EllipseParams& ellipseParams);
OpSet arc(double x, double y, double width, double height, double start, double stop, bool closed, bool roughClosure, const ResolvedOptions& o);

#if 0
// parsePath, normalize, absolutize
#include "path-data-parser.h"
OpSet svgPath(string path, const ResolvedOptions& o);
#endif

// Fills

OpSet solidFillPolygon(const std::vector<Point>& points, const ResolvedOptions& o);
OpSet patternFillPolygon(const std::vector<Point>& points, ResolvedOptions& o);
OpSet patternFillArc(double x, double y, double width, double height, double start, double stop, const ResolvedOptions& o);
double randOffset(double x, const ResolvedOptions& o);
double randOffsetWithRange(double min, double max, const ResolvedOptions& o);
std::vector<Op> doubleLineFillOps(double x1, double y1, double x2, double y2, ResolvedOptions& o);
