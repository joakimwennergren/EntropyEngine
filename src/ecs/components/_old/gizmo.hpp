#pragma once

#include <graphics/primitives/3d/plane.hpp>

using namespace Entropy::Primitives;

namespace Entropy {
namespace Components {
struct Gizmo {
  std::shared_ptr<Plane> plane;
};
}  // namespace Components
}  // namespace Entropy