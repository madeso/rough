#include "rough/svg.h"

svg::Document line_test()
{
  auto rc = RoughSVG{};
  rc.line(10, 10, 100, 10);
  rc.line(10, 210, 500, 210);
  rc.line(10, 20, 10, 110, Options{}.set_stroke("red"));
  rc.line(10, 10, 100, 10);
  rc.line(50, 30, 200, 100, Options{}.set_stroke("blue").set_strokeWidth(5));

  return rc.doc;
}
