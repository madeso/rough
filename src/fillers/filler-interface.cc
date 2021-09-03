#include "filler-interface.h"

std::vector<Op> RenderHelper::doubleLineOps(const Point& p1, const Point& p2, const ResolvedOptions& o)
{
    return doubleLineOps(p1.x, p1.y, p2.x, p2.y, o);
}

std::vector<Op> RenderHelper::doubleLineOps(const Line& line, const ResolvedOptions& o)
{
    return doubleLineOps(line.from, line.to, o);
}
