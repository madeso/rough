#pragma once

#include <optional>
#include <string>
#include <vector>

#include "rough/random.h"
#include "rough/geometry.h"

// const SVGNS = 'http://www.w3.org/2000/svg';

struct DrawingSurface {
  // todo(Gustav): figure this out
  double width; // also a SVGAnimatedLength?
  double height; // also a SVGAnimatedLength?
};

enum class FillStyle { solid, zigzag, crossHatch, dots, dashed, zigzagLine, hachure };

struct Options {  
  std::optional<double> simplification;
  std::optional<std::vector<double>> strokeLineDash;
  std::optional<double> strokeLineDashOffset;
  std::optional<std::vector<double>> fillLineDash;
  std::optional<double> fillLineDashOffset;
  std::optional<int> fixedDecimalPlaceDigits;

  std::optional<std::string> fill;
  double maxRandomnessOffset = 2; // or 1
  double roughness = 1;
  double bowing = 1;
  std::string stroke = "#000";
  double strokeWidth = 1;
  double curveFitting = 0;
  double curveTightness = 0.95;
  double curveStepCount = 9;
  FillStyle fillStyle = FillStyle::hachure;
  double fillWeight = -1;
  double hachureAngle = -41;
  double hachureGap = -1;
  double dashOffset = -1;
  double dashGap = -1;
  double zigzagOffset = -1;
  int seed = 0;
  bool combineNestedSvgPaths = false;
  bool disableMultiStroke = false;
  bool disableMultiStrokeFill = false;
  bool preserveVertices = false;
  mutable std::optional<Random> randomizer;

  Options& set_fill(const std::string& str);
  Options& set_fillStyle(FillStyle s);
  Options& set_hachureGap(double i);
  Options& set_fillWeight(double i);
  Options& set_roughness(int i);
  Options& set_hachureAngle(int i);
  Options& set_stroke(const std::string& str);
  Options& set_strokeWidth(int i);
};

enum class OpType { move, bcurveTo, lineTo};
enum class OpSetType { path, fillPath, fillSketch};

struct Op {
  OpType op;
  std::vector<double> data;
};

struct OpSet {
  OpSetType type;
  std::vector<Op> ops;
  std::optional<Point> size;
  std::optional<std::string> path;
};

enum class ShapeType {circle, line, rectangle, ellipse, linearPath, arc, curve, polygon };

struct Drawable {
  ShapeType shape;
  Options options;
  std::vector<OpSet> sets;
};

struct PathInfo {
  std::string d;
  std::string stroke;
  double strokeWidth;
  std::optional<std::string> fill;
};
