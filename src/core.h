#pragma once

#include <optional>
#include <string>
#include <vector>

#include "random.h"
#include "geometry.h"

// const SVGNS = 'http://www.w3.org/2000/svg';

struct DrawingSurface {
  // todo(Gustav): figure this out
  double width; // also a SVGAnimatedLength?
  double height; // also a SVGAnimatedLength?
};

enum class FillStyle { zigzag, crossHatch, dots, dashed, zigzagLine, hachure };

struct Options {
  std::optional<double> maxRandomnessOffset;
  std::optional<double> roughness;
  std::optional<double> bowing;
  std::optional<std::string> stroke;
  std::optional<double> strokeWidth;
  std::optional<double> curveFitting;
  std::optional<double> curveTightness;
  std::optional<double> curveStepCount;
  std::optional<std::string> fill;
  std::optional<FillStyle> fillStyle; // default hachure
  std::optional<double> fillWeight;
  std::optional<double> hachureAngle;
  std::optional<double> hachureGap;
  std::optional<double> simplification;
  std::optional<double> dashOffset;
  std::optional<double> dashGap;
  std::optional<double> zigzagOffset;
  std::optional<double> seed;
  std::optional<bool> combineNestedSvgPaths;
  std::optional<std::vector<double>> strokeLineDash;
  std::optional<double> strokeLineDashOffset;
  std::optional<std::vector<double>> fillLineDash;
  std::optional<double> fillLineDashOffset;
  std::optional<bool> disableMultiStroke;
  std::optional<bool> disableMultiStrokeFill;
  std::optional<bool> preserveVertices;
  std::optional<double> fixedDecimalPlaceDigits;
};

struct Config {
  std::optional<Options> options;
};

struct ResolvedOptions {
  double maxRandomnessOffset;
  double roughness;
  double bowing;
  std::string stroke;
  double strokeWidth;
  double curveFitting;
  double curveTightness;
  double curveStepCount;
  FillStyle fillStyle;
  double fillWeight;
  double hachureAngle;
  double hachureGap;
  double dashOffset;
  double dashGap;
  double zigzagOffset;
  double seed;
  bool combineNestedSvgPaths;
  std::optional<Random> randomizer;
  bool disableMultiStroke;
  bool disableMultiStrokeFill;
  bool preserveVertices;
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

struct Drawable {
  std::string shape;
  ResolvedOptions options;
  std::vector<OpSet> sets;
};

struct PathInfo {
  std::string d;
  std::string stroke;
  double strokeWidth;
  std::optional<std::string> fill;
};
