#include <optional>

#include "core.h"
#include "geometry.h"
#include "renderer.h"
#include "math.h"

const std::string NOS = "none";

#define private

struct RoughGenerator
{
  static double newSeed() {
    return randomSeed();
  }

  private Drawable _d(ShapeType shape, std::vector<OpSet> sets, Options options) {
    return { shape, options, sets};
  }

  Drawable line(double x1, double y1, double x2, double y2, const Options& options={}) {
    auto o = options;
    return this->_d(ShapeType::line, {::line(x1, y1, x2, y2, o)}, o);
  }

  Drawable rectangle(double x, double y, double width, double height, const Options& options={}) {
    auto o = options;
    std::vector<OpSet> paths ;
    const auto outline = ::rectangle(x, y, width, height, o);
    if (o.fill) {
      std::vector<Point> points= {{x, y}, {x + width, y}, {x + width, y + height}, {x, y + height}};
      if (o.fillStyle == FillStyle::solid) {
        paths.emplace_back(solidFillPolygon(points, o));
      } else {
        paths.emplace_back(patternFillPolygon(points, o));
      }
    }
    if (o.stroke != NOS) {
      paths.emplace_back(outline);
    }
    return this->_d(ShapeType::rectangle, paths, o);
  }

  Drawable ellipse(double x, double y, double width, double height, const Options& options={}) {
    auto o = options;
    std::vector<OpSet> paths;
    const auto ellipseParams = generateEllipseParams(width, height, o);
    const auto ellipseResponse = ellipseWithParams(x, y, o, ellipseParams);
    if (o.fill) {
      if (o.fillStyle == FillStyle::solid) {
        auto shape = ellipseWithParams(x, y, o, ellipseParams).opset;
        shape.type = OpSetType::fillPath;
        paths.emplace_back(shape);
      } else {
        paths.emplace_back(patternFillPolygon(ellipseResponse.estimatedPoints, o));
      }
    }
    if (o.stroke != NOS) {
      paths.emplace_back(ellipseResponse.opset);
    }
    return this->_d(ShapeType::ellipse, paths, o);
  }

  Drawable circle(double x, double y, double diameter, const Options& options={}) {
    auto ret = this->ellipse(x, y, diameter, diameter, options);
    ret.shape = ShapeType::circle;
    return ret;
  }

  Drawable linearPath(std::vector<Point> points, const Options& options={}) {
    auto o = options;
    return this->_d(ShapeType::linearPath, {::linearPath(points, false, o)}, o);
  }

  Drawable arc(double x, double y, double width, double height, double start, double stop, bool closed= false, const Options& options={}) {
    auto o = options;
    std::vector<OpSet> paths ;
    const auto outline = ::arc(x, y, width, height, start, stop, closed, true, o);
    if (closed && o.fill) {
      if (o.fillStyle == FillStyle::solid) {
        auto shape = ::arc(x, y, width, height, start, stop, true, false, o);
        shape.type = OpSetType::fillPath;
        paths.emplace_back(shape);
      } else {
        paths.emplace_back(patternFillArc(x, y, width, height, start, stop, o));
      }
    }
    if (o.stroke != NOS) {
      paths.emplace_back(outline);
    }
    return this->_d(ShapeType::arc, paths, o);
  }

  Drawable polygon(std::vector<Point> points, const Options& options={}) {
    auto o = options;
    std::vector<OpSet>  paths;
    const auto outline = ::linearPath(points, true, o);
    if (o.fill) {
      if (o.fillStyle == FillStyle::solid) {
        paths.emplace_back(solidFillPolygon(points, o));
      } else {
        paths.emplace_back(patternFillPolygon(points, o));
      }
    }
    if (o.stroke != NOS) {
      paths.emplace_back(outline);
    }
    return this->_d(ShapeType::polygon, paths, o);
  }
};
