#include "rough/core.h"

Options& Options::set_fill(const std::string& new_value)
{
    fill = new_value;
    return *this;
}

Options& Options::set_fillStyle(FillStyle new_value)
{
    fillStyle = new_value;
    return *this;
}

Options& Options::set_hachureGap(double new_value)
{
    hachureGap = new_value;
    return *this;
}

Options& Options::set_fillWeight(double new_value)
{
    fillWeight = new_value;
    return *this;
}

Options& Options::set_roughness(int new_value)
{
    roughness = new_value;
    return *this;
}

Options& Options::set_hachureAngle(int new_value)
{
    hachureAngle = new_value;
    return *this;
}

Options& Options::set_stroke(const std::string& new_value)
{
    stroke = new_value;
    return *this;
}

Options& Options::set_strokeWidth(int new_value)
{
  strokeWidth = new_value;
  return *this;
}
