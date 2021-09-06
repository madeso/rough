#include "rough/svg.h"

svg::Document rectangle_test()
{
  auto rc = RoughSVG{};
  rc.rectangle(10, 10, 80, 80);
  rc.rectangle(110, 10, 80, 80, Options{}.set_fill("red"));
  rc.rectangle(210, 10, 80, 80, Options{}.set_fill("pink").set_fillStyle(FillStyle::solid));
  rc.rectangle(310, 10, 80, 80, Options{}.set_fill("red").set_fillStyle(FillStyle::crossHatch));
  rc.rectangle(410, 10, 80, 80, Options{}.set_fill("red").set_fillStyle(FillStyle::zigzag).set_hachureGap(8));
  rc.rectangle(510, 10, 80, 80, Options{}.set_fill("red").set_fillStyle(FillStyle::dots));

  rc.rectangle(10, 110, 80, 80, Options{}.set_roughness(2));
  rc.rectangle(110, 110, 80, 80, Options{}.set_fill("red").set_stroke("blue").set_hachureAngle(0).set_strokeWidth(3));
  rc.rectangle(210, 110, 80, 80, Options{}.set_fill("pink").set_fillWeight(5).set_hachureGap(10).set_hachureAngle(90));

  rc.rectangle(10, 210, 480, 280, Options{}.set_fill("red").set_fillStyle(FillStyle::dots).set_hachureGap(20).set_fillWeight(2));

  return rc.doc;
}
