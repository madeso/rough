#pragma once

struct PatternFiller;
struct ResolvedOptions;
struct RenderHelper;

#include <memory>

std::unique_ptr<PatternFiller> getFiller(const ResolvedOptions& o, RenderHelper* helper);
