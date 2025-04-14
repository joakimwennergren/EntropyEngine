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

#ifndef ENTROPY_VULKAN_VERTEX_BUFFER_H
#define ENTROPY_VULKAN_VERTEX_BUFFER_H

#include "base_buffer.h"
#include "vulkan/data/vertex.h"

namespace Entropy::Vulkan::Buffers {

/**
 * @class VertexBuffer
 * @brief A class that represents a buffer for storing vertex data on the GPU.
 *
 * The VertexBuffer class is used to handle vertex data in graphics pipelines.
 * It provides operations for binding, unbinding, and managing memory for
 * vertex attribute data, enhancing performance in rendering operations.
 *
 * The buffer is typically used in conjunction with other graphics resources,
 * such as shaders and vertex array objects (VAOs), to define the geometry
 * and appearance of rendered objects.
 *
 * Features include:
 * - Uploading vertex data to the GPU.
 * - Binding and unbinding vertex buffer objects.
 * - Efficient memory management for vertex data.
 *
 * This class ensures proper interaction with graphical APIs (e.g., OpenGL,
 * Vulkan) by abstracting low-level buffer operations.
 */
template <class T>
class VertexBuffer final : public BaseBuffer {
 public:
  explicit VertexBuffer(VkDeviceSize size);
  void Update(std::vector<T> vertices);
};
}  // namespace Entropy::Vulkan::Buffers

#endif  // ENTROPY_VULKAN_VERTEX_BUFFER_H
