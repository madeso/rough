#include "geometry.h"

#include <limits>
#include <cmath>
#include <stdexcept>

#include "double.h"


const double& Point::operator[](const int index) const
{
  switch(index)
  {
    case 0:
      return x;
    case 1:
      return y;
    default:
      throw std::out_of_range("Point::operator[]");
  }
}

double& Point::operator[](const int index)
{
  switch(index)
  {
    case 0:
      return x;
    case 1:
      return y;
    default:
      throw std::out_of_range("Point::operator[]");
  }
}


// local functions
namespace
{
	// Check if q lies on the line segment pr
	bool onSegment(const Point& p, const Point& q, const Point& r) {
	  return (
	    q.x <= std::max(p.x, r.x) &&
	    q.x >= std::min(p.x, r.x) &&
	    q.y <= std::max(p.y, r.y) &&
	    q.y >= std::min(p.y, r.y)
	  );
	}

	// For the ordered points p, q, r, return
	// 0 if p, q, r are collinear
	// 1 if Clockwise
	// 2 if counterclickwise
	int orientation(const Point& p, const Point& q, const Point& r) {
	  const auto val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
	  if (isZero(val)) {
	    return 0;
	  }
	  return val > 0 ? 1 : 2;
	}
}

bool isSame(const Point& lhs, const Point& rhs) {
  return isSame(lhs.x, rhs.x) && isSame(lhs.y, rhs.y);
}

bool operator==(const Point& lhs, const Point& rhs)
{
  return isSame(lhs, rhs);
}

bool operator!=(const Point& lhs, const Point& rhs)
{
  return !isSame(lhs, rhs);
}

template<typename Points, typename GetX, typename GetY, typename SetX, typename SetY>
void tRotatePoints(Points* points, const Point& center, double degrees, GetX&& getx, GetY&& gety, SetX&& setx, SetY&& sety) {
  if (!points->empty()) {
    const auto cx = center.x;
    const auto cy = center.y;
    const auto angle = (pi / 180) * degrees;
    const auto cos = std::cos(angle);
    const auto sin = std::sin(angle);
    for(auto& p: *points)
    {
      const auto x = getx(p);
      const auto y = gety(p);
      setx(p, ((x - cx) * cos) - ((y - cy) * sin) + cx);
      sety(p, ((x - cx) * sin) + ((y - cy) * cos) + cy);
    }
  }
}

void rotatePoints(std::vector<Point>* points, const Point& center, double degrees) {
  tRotatePoints(points, center, degrees,
      [](const Point& p){return p.x;}, [](const Point& p){return p.y;},
      [](Point& p, double d){return p.x = d;}, [](Point& p, double d){p.y = d;}
  );
}

void rotateLines(std::vector<Line>* lines, const Point& center, double degrees) {
  std::vector<Point*> points;
  for(auto& l: *lines) {
    points.emplace_back(&l.from);
    points.emplace_back(&l.to);
  }
  tRotatePoints(&points, center, degrees,
      [](const Point* p){return p->x;}, [](const Point* p){return p->y;},
      [](Point* p, double d){return p->x = d;}, [](Point* p, double d){p->y = d;}
  );
}

double lineLength(const Line& line) {
  const auto& p1 = line.from;
  const auto& p2 = line.to;
  return std::sqrt(std::pow(p1.x - p2.x, 2) + std::pow(p1.y - p2.y, 2));
}


std::optional<Point> lineIntersection(const Point& a, const Point& b, const Point& c, const Point& d) {
  const auto a1 = b.y - a.y;
  const auto b1 = a.x - b.x;
  const auto c1 = a1 * (a.x) + b1 * (a.y);
  const auto a2 = d.y - c.y;
  const auto b2 = c.x - d.x;
  const auto c2 = a2 * (c.x) + b2 * (c.y);
  const auto determinant = a1 * b2 - a2 * b1;
  if (!isZero(determinant)) return Point{ (b2 * c1 - b1 * c2) / determinant, (a1 * c2 - a2 * c1) / determinant };
  else return std::nullopt;
}

bool isPointInPolygon(const std::vector<Point>& points, double x, double y) {
  const auto vertices = points.size();

  // There must be at least 3 vertices in polygon
  if (vertices < 3) {
    return false;
  }
  const auto extreme = Point{std::numeric_limits<double>::max(), y};
  const auto p = Point{x, y};
  std::size_t count = 0;
  for (std::size_t i = 0; i < vertices; i++) {
    const auto& current = points[i];
    const auto& next = points[(i + 1) % vertices];
    if (doIntersect(current, next, p, extreme)) {
      if (orientation(current, p, next) == 0) {
        return onSegment(current, p, next);
      }
      count++;
    }
  }
  // true if count is off
  return (count % 2) == 1;
}

bool isPointInPolygon(const std::vector<Point>& points, const Point& p)
{
  return isPointInPolygon(points, p.x, p.y);
}

// Check is p1q1 intersects with p2q2
bool doIntersect(const Point& p1, const Point& q1, const Point& p2, const Point& q2) {
  const auto o1 = orientation(p1, q1, p2);
  const auto o2 = orientation(p1, q1, q2);
  const auto o3 = orientation(p2, q2, p1);
  const auto o4 = orientation(p2, q2, q1);

  if (o1 != o2 && o3 != o4) {
    return true;
  }

  // p1, q1 and p2 are colinear and p2 lies on segment p1q1
  if (o1 == 0 && onSegment(p1, p2, q1)) {
    return true;
  }

  // p1, q1 and p2 are colinear and q2 lies on segment p1q1
  if (o2 == 0 && onSegment(p1, q2, q1)) {
    return true;
  }

  // p2, q2 and p1 are colinear and p1 lies on segment p2q2
  if (o3 == 0 && onSegment(p2, p1, q2)) {
    return true;
  }

  // p2, q2 and q1 are colinear and q1 lies on segment p2q2
  if (o4 == 0 && onSegment(p2, q1, q2)) {
    return true;
  }

  return false;
}

bool doIntersect(const Point& p1, const Point& q1, const Line& line)
{
  return doIntersect(p1, q1, line.from, line.to);
}
