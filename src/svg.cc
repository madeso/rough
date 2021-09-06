#include <map>
#include <sstream>

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
    void add(const std::shared_ptr<Element>& element)
    {
      elements.emplace_back(element);
    }
  };

  struct Element
  {
    std::string name;
    std::map<std::string, std::string> attributes;
    std::vector<std::shared_ptr<Element>> children;

    Element(const std::string& name) : name(name) {}

    void setAttribute(const std::string& attribute_name, const std::string& value)
    {
      attributes[attribute_name] = value;
    }

    void appendChild(const std::shared_ptr<Element>& element)
    {
      children.emplace_back(element);
    }
  };

  using ElementRef = std::shared_ptr<Element>;

  ElementRef make_element(const std::string& name)
  {
    return std::make_shared<Element>(name);
  }
}

struct RoughSVG {
  RoughGenerator gen;
  svg::Document doc;

  void draw(const Drawable& drawable) {
    const auto& sets = drawable.sets;
    const auto& o = drawable.options;
    const auto g = svg::make_element("g");
    const auto precision = drawable.options.fixedDecimalPlaceDigits;
    for (const auto& drawing: sets) {
      svg::ElementRef path;
      switch (drawing.type) {
        case OpSetType::path:
          path = svg::make_element("h");
          path->setAttribute("d", this->opsToPath(drawing, precision));
          path->setAttribute("stroke", o.stroke);
          path->setAttribute("stroke-width", std::to_string(o.strokeWidth));
          path->setAttribute("fill", "none");
          if (o.strokeLineDash) {
            path->setAttribute("stroke-dasharray", join(*o.strokeLineDash, " "));
          }
          if (o.strokeLineDashOffset) {
            path->setAttribute("stroke-dashoffset", std::to_string(*o.strokeLineDashOffset));
          }
          break;
        case OpSetType::fillPath:
          path = svg::make_element("h");
          path->setAttribute("d", this->opsToPath(drawing, precision));
          path->setAttribute("stroke", "none");
          path->setAttribute("stroke-width", "0");
          path->setAttribute("fill", o.fill.value_or(""));
          if (drawable.shape == ShapeType::curve || drawable.shape == ShapeType::polygon) {
            path->setAttribute("fill-rule", "evenodd");
          }
          break;
        case OpSetType::fillSketch:
          path = this->fillSketch(drawing, o);
          break;
      }
      if (path) {
        g->appendChild(path);
      }
    }
    doc.add(g);
  }

  svg::ElementRef fillSketch(const OpSet& drawing, const Options& o) {
    auto fweight = o.fillWeight;
    if (fweight < 0) {
      fweight = o.strokeWidth / 2;
    }
    auto path = svg::make_element("h");
    path->setAttribute("d", this->opsToPath(drawing, o.fixedDecimalPlaceDigits));
    path->setAttribute("stroke", o.fill.value_or(""));
    path->setAttribute("stroke-width", std::to_string(fweight));
    path->setAttribute("fill", "none");
    if (o.fillLineDash) {
      path->setAttribute("stroke-dasharray", join(*o.fillLineDash, " "));
    }
    if (o.fillLineDashOffset) {
      path->setAttribute("stroke-dashoffset", std::to_string(*o.fillLineDashOffset));
    }
    return path;
  }

  std::string opsToPath(const OpSet& drawing, std::optional<int>fixedDecimals) {
    std::ostringstream path;
    for (const auto& item: drawing.ops) {
      const auto& data = item.data;
      const auto str = [&fixedDecimals](double d) -> std::string {
        if(fixedDecimals) {
          std::ostringstream out;
          out.precision(*fixedDecimals);
          out << std::fixed << d;
          return out.str();
        }
        else {
          return std::to_string(d);
        }
      };
      switch (item.op) {
        case OpType::move:
          path << 'M' << str(data[0]) << ' ' << str(data[1]) << ' ';
          break;
        case OpType::bcurveTo:
          path << 'C' << str(data[0]) << ' ' << str(data[1]) << ", " << str(data[2]) << ' ' << str(data[3]) << ", " << str(data[4]) << ' ' << str(data[5]) << ' ';
          break;
        case OpType::lineTo:
          path << 'L' << str(data[0]) << ' ' << str(data[1]) << ' ';
          break;
      }
    }
    return path.str();
  }

  void line(double x1, double y1, double x2, double y2, const Options& options = {}) {
    const auto d = this->gen.line(x1, y1, x2, y2, options);
    this->draw(d);
  }

  void rectangle(double x, double y, double width, double height, const Options& options = {}) {
    const auto d = this->gen.rectangle(x, y, width, height, options);
    this->draw(d);
  }

  void ellipse(double x, double y, double width, double height, const Options& options = {}) {
    const auto d = this->gen.ellipse(x, y, width, height, options);
    this->draw(d);
  }

  void circle(double x, double y, double diameter, const Options& options = {}) {
    const auto d = this->gen.circle(x, y, diameter, options);
    this->draw(d);
  }

  void linearPath(const std::vector<Point>& points, const Options& options = {}) {
    const auto d = this->gen.linearPath(points, options);
    this->draw(d);
  }

  void polygon(const std::vector<Point>& points, const Options& options = {}) {
    const auto d = this->gen.polygon(points, options);
    this->draw(d);
  }

  void arc(double x, double y, double width, double height, double start, double stop, bool closed= false, const Options& options = {}) {
    const auto d = this->gen.arc(x, y, width, height, start, stop, closed, options);
    this->draw(d);
  }

  // void curve(const std::vector<Point>& points, const Options& options = {}) {
  //   const auto d = this->gen.curve(points, options);
  //   this->draw(d);
  // }

  // void path(const std::string& d, const Options& options = {}) {
  //   const drawing = this->gen.path(d, options);
  //   this->draw(drawing);
  // }
};
