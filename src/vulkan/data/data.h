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

#ifndef ENTROPY_VULKAN_DATA_H
#define ENTROPY_VULKAN_DATA_H

#include <glm/glm.hpp>

namespace Entropy::Graphics::Vulkan::Data {

struct UBOData {
  glm::mat4 perspective;
  glm::mat4 view;
};

struct InstanceData {
  glm::mat4 model;
  glm::vec4 bgColor;
  glm::vec4 borderColor;
  glm::vec4 cornerRadiuses;
  glm::vec2 dimension;
  int type;
  int textureIndex;
};

} // namespace Entropy::Graphics::Vulkan::Data

#endif // ENTROPY_VULKAN_DATA_H
