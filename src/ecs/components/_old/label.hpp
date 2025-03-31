#pragma once

#include <graphics/text/label.hpp>

using namespace Entropy::Graphics::Primitives;

namespace Entropy {
namespace Components {
struct LabelComponent {
  std::shared_ptr<Label> label;
};
} // namespace Components
} // namespace Entropy
