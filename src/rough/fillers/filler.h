#pragma once

#include <memory>

struct PatternFiller;
struct Options;
struct RenderHelper;

std::unique_ptr<PatternFiller> getFiller(const Options& o, RenderHelper* helper);
