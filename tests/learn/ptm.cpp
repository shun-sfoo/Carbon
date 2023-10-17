#include <iostream>

// 成员指针

// 一个指向类C非静态成员m的成员指针可以用&C::m进行初始化
// 标准规定，&C::m表示成员指针，&(C::m)或者&m表示对m成员取地址

struct C {
  int m;

  void f() {
    int C::*x1 = &C::m; // pointer to member m of C
    int *x2 = &(C::m);  // pointer to member this->m
  }
};

int main() {
  // p是类指向类C成员m的指针，
  int C::*p = &C::m;
  // 固定格式 T U::*
  // T是它指向的成员的类型，U是它指向的类的类型
  // 这里就是int C::* ：指向类C的int成员
  C c = {7};

  std::cout << c.*p << '\n';
  C *cp = &c;
  cp->m = 10;
  std::cout << cp->*p << std::endl;
}
