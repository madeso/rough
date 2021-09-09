#pragma once

#include <optional>

int randomSeed();

struct Random
{
  int seed;

  explicit Random(int seed);

  double next();
};
