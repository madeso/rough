#pragma once

#include <map>

#include "core.h"
#include "generator.h"
#include "geometry.h"
#include "vector-utils.h"

namespace svg
{
  struct Element;

  struct Document
  {
    std::vector<std::shared_ptr<Element>> elements;

    void add(const std::shared_ptr<Element>& element);
  };

  struct Element
  {
    std::string name;
    std::map<std::string, std::string> attributes;
    std::vector<std::shared_ptr<Element>> children;

    Element(const std::string& name);

    void setAttribute(const std::string& attribute_name, const std::string& value);
    void appendChild(const std::shared_ptr<Element>& element);
  };

  using ElementRef = std::shared_ptr<Element>;

  ElementRef make_element(const std::string& name);

}

struct RoughSVG {
  RoughGenerator gen;
  svg::Document doc;

  void draw(const Drawable& drawable);
  svg::ElementRef fillSketch(const OpSet& drawing, const Options& o);
  std::string opsToPath(const OpSet& drawing, std::optional<int>fixedDecimals);

  void line(double x1, double y1, double x2, double y2, const Options& options = {});
  void rectangle(double x, double y, double width, double height, const Options& options = {});
  void ellipse(double x, double y, double width, double height, const Options& options = {});
  void circle(double x, double y, double diameter, const Options& options = {});
  void linearPath(const std::vector<Point>& points, const Options& options = {});
  void polygon(const std::vector<Point>& points, const Options& options = {});
  void arc(double x, double y, double width, double height, double start, double stop, bool closed= false, const Options& options = {});
};
