#include <iostream>

// 指向基类的数据成员指针 可以隐式转换成 非虚继承的派生类数据成员指针
struct Base {
  int m;
};

struct Derived1 : Base {};         // 非虚继承
struct Derived2 : virtual Base {}; // 虚继承

int main() {
  int Base::*bp = &Base::m;
  int Derived1::*dp = bp;
  // int Derived2::* dp2 = bp;  error
  Derived1 d;
  d.m = 1;
  std::cout << d.*dp << ' ' << d.*bp << std::endl;
}
