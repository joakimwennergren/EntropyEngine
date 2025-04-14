#pragma once

#include <graphics/primitives/2d/sprite.hpp>

namespace Entropy {
namespace Components {
struct SpriteComponent {
  std::shared_ptr<Entropy::Primitives::Sprite> sprite;
};
}  // namespace Components
}  // namespace Entropy