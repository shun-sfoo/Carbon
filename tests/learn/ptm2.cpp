#include <iostream>

struct Point {
  int x;
  int y;
};

auto &access(Point &point, int Point::*pm) { return point.*pm; }

int main() {
  Point point;
  access(point, &Point::x) = 10;
  access(point, &Point::y) = 20;
  std::cout << point.x << ' ' << point.y << std::endl; // 10 20
}
