#pragma once

#include <graphics/primitives/2d/line.hpp>
#include "graphics/vulkan/textures/texture.hpp"

using namespace Entropy::Primitives;

namespace Entropy {
namespace Components {
struct HasTexture {
  std::shared_ptr<Texture> texture;
};
}  // namespace Components
}  // namespace Entropy
