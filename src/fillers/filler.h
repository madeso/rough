#pragma once

struct PatternFiller;
struct Options;
struct RenderHelper;

#include <memory>

std::unique_ptr<PatternFiller> getFiller(const Options& o, RenderHelper* helper);
