// Copyright 2025 Joakim Wennergren
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#ifndef ENTROPY_ECS_COMPONENT_SPRITE_H
#define ENTROPY_ECS_COMPONENT_SPRITE_H

#include <glm/glm.hpp>
#include "ecs/components/position.h"
#include "ecs/components/dimension.h"
#include "ecs/components/texture.h"
#include "ecs/components/2d_quad.h"

namespace Entropy::ECS::Components {
struct Sprite {
  TwoDQuad two_d_quad;
  Texture texture;
  Position position;
  Dimension dimension;
  glm::vec4 color;
};
} // namespace Entropy::ECS::Components

#endif // ENTROPY_ECS_COMPONENT_SPRITE_H
