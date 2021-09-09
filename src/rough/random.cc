#include "rough/random.h"

#include <random>
#include <cstdint>

namespace
{
  std::mt19937& random_generator()
  {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    return gen;
  }

  int generate_random_int()
  {
    std::uniform_int_distribution<> dis(0, std::numeric_limits<int>::max());
    return dis(random_generator());
  }

  double generate_random_double()
  {
    std::uniform_real_distribution<> dis(0.0, 1.0);
    return dis(random_generator());
  }
}
\
int randomSeed() {
  return generate_random_int();
}

Random::Random(int s)
  : seed(s)
{
}

double Random::next() {
  if (seed != 0) {
    constexpr std::int64_t big = 0x80000000;
    seed = 48271 * seed;
    return static_cast<double>((big - 1) & seed) / static_cast<double>(big);
  } else {
    return generate_random_double();
  }
}
