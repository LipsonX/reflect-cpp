#include <cassert>
#include <iostream>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <source_location>
#include <string>
#include <vector>

#include "test_enum.hpp"
#include "write_and_read.hpp"

namespace test_flag_enum {

enum class Color {
  red = 256,
  green = 512,
  blue = 1024,
  yellow = 2048,
  orange = (256 | 2048)  // red + yellow = orange
};

inline Color operator|(Color c1, Color c2) {
  return static_cast<Color>(static_cast<int>(c1) | static_cast<int>(c2));
}

struct Circle {
  float radius;
  Color color;
};

void test() {
  std::cout << std::source_location::current().function_name() << std::endl;

  const auto circle =
      Circle{.radius = 2.0, .color = Color::blue | Color::orange};

  write_and_read(circle, R"({"radius":2.0,"color":"red|blue|yellow"})");

  auto mutable_circle = circle;
  rfl::get_enumerators<Color>().apply([&](auto field) {
    if constexpr (decltype(field)::name() == "green") {
      mutable_circle.color = field.value();
    }
  });
  write_and_read(mutable_circle, R"({"radius":2.0,"color":"green"})");
}

}  // namespace test_flag_enum
