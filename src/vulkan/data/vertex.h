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

#ifndef ENTROPY_VULKAN_VERTEX_H
#define ENTROPY_VULKAN_VERTEX_H

#include <array>
#include <glm/glm.hpp>
#include <vulkan/vulkan.hpp>
#include "data.h"

namespace Entropy::Vulkan::Data {

struct TwoDVertex {
  glm::vec3 pos;
  glm::vec2 uv;

  static std::array<VkVertexInputBindingDescription, 2>
  getBindingDescriptions() {
    std::array<VkVertexInputBindingDescription, 2> bindingDescriptions{};
    bindingDescriptions[0].binding = 0;
    bindingDescriptions[0].stride = sizeof(TwoDVertex);
    bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    bindingDescriptions[1].binding = 1;
    bindingDescriptions[1].stride = sizeof(InstanceDataTwoD);
    bindingDescriptions[1].inputRate = VK_VERTEX_INPUT_RATE_INSTANCE;

    return bindingDescriptions;
  }

  static std::array<VkVertexInputAttributeDescription, 7>
  getAttributeDescriptions() {
    std::array<VkVertexInputAttributeDescription, 7> attributeDescriptions{};

    attributeDescriptions[0].binding = 0;
    attributeDescriptions[0].location = 0;
    attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[0].offset = offsetof(TwoDVertex, pos);

    attributeDescriptions[1].binding = 0;
    attributeDescriptions[1].location = 1;
    attributeDescriptions[1].format = VK_FORMAT_R32G32_SFLOAT;
    attributeDescriptions[1].offset = offsetof(TwoDVertex, uv);

    attributeDescriptions[2].binding = 1;
    attributeDescriptions[2].location = 2;
    attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
    attributeDescriptions[2].offset = 0;

    attributeDescriptions[3].binding = 1;
    attributeDescriptions[3].location = 3;
    attributeDescriptions[3].format = VK_FORMAT_R32G32_SFLOAT;
    attributeDescriptions[3].offset = sizeof(float) * 2;

    attributeDescriptions[4].binding = 1;
    attributeDescriptions[4].location = 4;
    attributeDescriptions[4].format = VK_FORMAT_R32_SFLOAT;
    attributeDescriptions[4].offset = sizeof(float) * 4;

    attributeDescriptions[5].binding = 1;
    attributeDescriptions[5].location = 5;
    attributeDescriptions[5].format = VK_FORMAT_R32_SINT;
    attributeDescriptions[5].offset = sizeof(int32_t) * 5;

    attributeDescriptions[6].binding = 1;
    attributeDescriptions[6].location = 6;
    attributeDescriptions[6].format = VK_FORMAT_R32G32B32A32_SFLOAT;
    attributeDescriptions[6].offset = sizeof(float) * 6;

    return attributeDescriptions;
  }
};

struct ThreeDAnimVertex {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec4 color;
  glm::vec2 uv;
  glm::vec4 joint;
  glm::vec4 weight;

  static std::array<VkVertexInputBindingDescription, 1>
  getBindingDescriptions() {
    std::array<VkVertexInputBindingDescription, 1> bindingDescriptions{};
    bindingDescriptions[0].binding = 0;
    bindingDescriptions[0].stride = sizeof(ThreeDAnimVertex);
    bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    return bindingDescriptions;
  }

  static std::array<VkVertexInputAttributeDescription, 6>
  getAttributeDescriptions() {
    std::array<VkVertexInputAttributeDescription, 6> attributeDescriptions{};

    attributeDescriptions[0].binding = 0;
    attributeDescriptions[0].location = 0;
    attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[0].offset = offsetof(ThreeDAnimVertex, position);

    attributeDescriptions[1].binding = 0;
    attributeDescriptions[1].location = 1;
    attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[1].offset = offsetof(ThreeDAnimVertex, normal);

    attributeDescriptions[2].binding = 0;
    attributeDescriptions[2].location = 2;
    attributeDescriptions[2].format = VK_FORMAT_R32G32B32A32_SFLOAT;
    attributeDescriptions[2].offset = offsetof(ThreeDAnimVertex, color);

    attributeDescriptions[3].binding = 0;
    attributeDescriptions[3].location = 3;
    attributeDescriptions[3].format = VK_FORMAT_R32G32_SFLOAT;
    attributeDescriptions[3].offset = offsetof(ThreeDAnimVertex, uv);

    attributeDescriptions[4].binding = 0;
    attributeDescriptions[4].location = 4;
    attributeDescriptions[4].format = VK_FORMAT_R32G32B32A32_SFLOAT;
    attributeDescriptions[4].offset = offsetof(ThreeDAnimVertex, joint);

    attributeDescriptions[5].binding = 0;
    attributeDescriptions[5].location = 5;
    attributeDescriptions[5].format = VK_FORMAT_R32G32B32A32_SFLOAT;
    attributeDescriptions[5].offset = offsetof(ThreeDAnimVertex, weight);

    return attributeDescriptions;
  }
};
}  // namespace Entropy::Vulkan::Data

#endif  // ENTROPY_VULKAN_VERTEX_H
