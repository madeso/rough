#pragma once

#include <optional>

int randomSeed();

struct Random
{
  std::optional<int> seed;

  explicit Random(std::optional<int> seed);

  double next();
};
