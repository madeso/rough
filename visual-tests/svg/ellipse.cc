#include "rough/svg.h"

svg::Document ellipse_test()
{
  auto rc = RoughSVG{};
  rc.ellipse(10 + 40, 10 + 40, 80, 80);
  rc.ellipse(110 + 40, 10 + 40, 80, 80, Options{}.set_fill("red"));
  rc.ellipse(210 + 40, 10 + 40, 80, 80, Options{}.set_fill("pink").set_fillStyle(FillStyle::solid));
  rc.ellipse(310 + 40, 10 + 40, 80, 80, Options{}.set_fill("red").set_fillStyle(FillStyle::crossHatch));
  rc.ellipse(410 + 40, 10 + 40, 80, 80, Options{}.set_fill("red").set_fillStyle(FillStyle::zigzag).set_hachureGap(8));
  rc.ellipse(510 + 40, 10 + 40, 80, 80, Options{}.set_fill("red").set_fillStyle(FillStyle::dots));

  rc.circle(10 + 40, 110 + 40, 80, Options{}.set_roughness(2));
  rc.circle(110 + 40, 110 + 40, 80, Options{}.set_fill("red").set_stroke("blue").set_hachureAngle(0).set_strokeWidth(3));
  rc.circle(210 + 40, 110 + 40, 80, Options{}.set_fill("pink").set_fillWeight(3).set_hachureGap(8).set_hachureAngle(45));

  rc.ellipse(300, 350, 480, 280, Options{}.set_fill("red").set_fillStyle(FillStyle::dots).set_hachureGap(20).set_hachureAngle(0).set_fillWeight(2));

  return rc.doc;
}
