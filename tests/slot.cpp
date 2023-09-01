#include <Event1.hpp>
#include <EventHandler.hpp>
#include <ParameterCrop.hpp>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Slot", "[is_subset_of]") {
  REQUIRE(is_subset_of<std::tuple<>, std::tuple<>>::value);
  REQUIRE(is_subset_of<std::tuple<int, char>,
                       std::tuple<int, char, double>>::value);
  REQUIRE_FALSE(
      is_subset_of<std::tuple<int, char>, std::tuple<char, double>>::value);
}

TEST_CASE("Slot", "[find_next_index]") {
  REQUIRE(find_next_index<1, int, std::tuple<int, char, double, int>>::value ==
          3);
}

struct Key {
  int value = 10;
};

TEST_CASE("Slot", "[tuple take1]") {
  int result = 0;
  std::function<void(int, char, Key)> func = [&](int a, char b, Key key) {
    result += a + b + key.value;
  };
  std::tuple<int, int, char, int, float, Key> args = {1, 2, 10, 4, 3.0f, Key()};
  TupleTake(func, args);
  REQUIRE(result == 21);
}

TEST_CASE("Slot", "[tuple take2]") {
  int result = 0;
  std::function<void(char, int)> func = [&](char a, int b) { result += a + b; };

  std::tuple<int, int, char> args = {1, 2, 'a'};
  TupleTake(func, args);
  REQUIRE(result == 0);
}

int e = 0;

void add() { e += 1; }

class Button {
public:
  void add(char a) { e += a; }
};

TEST_CASE("Slot", "[event handler 1]") {
  EventHandler<void, std::tuple<int>, int, char> func([](int a) { e += a; });
  func(1, 'a');
  REQUIRE(e == 1);
  EventHandler<void, std::tuple<>, int, char> func1(add);
  func1(1, 'a');
  REQUIRE(e == 2);
  EventHandler<Button, std::tuple<char>, int, char> func2(new Button,
                                                          &Button::add);
  func2(1, 10);
  REQUIRE(e == 12);

  EventHandlerInterface<int, char> *handler = &func;
  (*handler)(1, 'a');
  REQUIRE(e == 13);
  handler = &func1;
  (*handler)(1, 'a');
  REQUIRE(e == 14);
  handler = &func2;
  (*handler)(1, 10);
  REQUIRE(e == 24);
}

int e1 = 0;

void addTwo(int x, int y) { e1 += x + y; }

class ButtonTwo {
public:
  void add(int a) { e1 += a; }

  Event<int, int> Clicked;
};

class Label {
public:
  void add(int a) { e1 += a; }
};

TEST_CASE("Slot", "[event 1]") {
  ButtonTwo *button = new ButtonTwo();
  Label *label = new Label();
  button->Clicked.Attach(addTwo);
  button->Clicked.Attach(button, &ButtonTwo::add);
  button->Clicked.Attach(label, [](int x, int y) { e1 += x + y; });
  // run addtow() add()  label ()   e1 += 1 + 2, 1, 1 + 2 = 7
  button->Clicked(1, 2);
  REQUIRE(e1 == 7);
  button->Clicked.Detach(addTwo);
  button->Clicked(1, 2); //  run add() label()  e1 = 7 += 1, 1+2 = 11
  REQUIRE(e1 == 11);
  button->Clicked.Detach(button, &ButtonTwo::add);
  button->Clicked(1, 2); //  run label()  e1 = 11 += 1 + 2 = 14
  REQUIRE(e1 == 14);
  button->Clicked.Clear();
  button->Clicked(1, 2); // no function e1 = 14;
  REQUIRE(e1 == 14);
  delete button;
  delete label;
}
