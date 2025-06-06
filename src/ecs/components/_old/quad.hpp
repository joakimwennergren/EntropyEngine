#pragma once

#include <graphics/primitives/2d/quad.hpp>

namespace Entropy {
namespace Components {
struct QuadComponent {
  std::shared_ptr<Entropy::Primitives::Quad> quad;
};
}  // namespace Components
}  // namespace Entropy