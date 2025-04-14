#pragma once

#include <graphics/primitives/2d/line.hpp>

using namespace Entropy::Primitives;

namespace Entropy::Components {
struct Color {
  glm::vec4 color;
  glm::vec4 borderColor;
};
}  // namespace Entropy::Components
