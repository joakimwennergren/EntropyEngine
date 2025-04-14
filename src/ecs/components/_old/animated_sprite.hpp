#pragma once

#include <graphics/primitives/2d/line.hpp>
#include "graphics/vulkan/textures/texture.hpp"

using namespace Entropy::Primitives;

namespace Entropy {
namespace Components {
struct HasAnimatedSprite {
  std::vector<std::shared_ptr<Texture>> textures;
};
}  // namespace Components
}  // namespace Entropy
