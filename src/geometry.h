#pragma once

#include <vector>
#include <optional>

struct Point
{
   double x;
   double y;
};

bool isSame(const Point& lhs, const Point& rhs);
bool operator==(const Point& lhs, const Point& rhs);
bool operator!=(const Point& lhs, const Point& rhs);

struct Line
{
  Point from;
  Point to;
};

struct Rectangle {
  double x;
  double y;
  double width;
  double height;
};

void rotatePoints(std::vector<Point>* points, const Point& center, double degrees);
void rotateLines(std::vector<Line>* lines, const Point& center, double degrees);
double lineLength(const Line& line);
std::optional<Point> lineIntersection(const Point& a, const Point& b, const Point& c, const Point& d);

// Check if p1q1 intersects with p2q2
bool doIntersect(const Point& p1, const Point& q1, const Point& p2, const Point& q2);
