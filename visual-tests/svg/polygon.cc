#include "rough/svg.h"

svg::Document polygon_test()
{
  auto rc = RoughSVG{};
  
    rc.polygon({
      Point{10, 10},
      Point{200, 10},
      Point{100, 100},
      Point{100, 50},
      Point{300, 100},
      Point{60, 200}
  }, Options{}.set_fillStyle(FillStyle::solid).set_stroke("black").set_strokeWidth(2).set_fill("red").set_hachureAngle(90));

  return rc.doc;
}
