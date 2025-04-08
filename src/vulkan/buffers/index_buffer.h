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

#ifndef ENTROPY_VULKAN_INDEX_BUFFER_H
#define ENTROPY_VULKAN_INDEX_BUFFER_H

#include "base_buffer.h"

namespace Entropy::Graphics::Vulkan::Buffers {

/**
 * @class IndexBuffer
 *
 * @brief Represents a buffer that stores indices for rendering in a graphics
 * application.
 *
 * An IndexBuffer is used in conjunction with a VertexBuffer to define the order
 * in which vertices should be rendered. It typically contains indices that
 * reference vertex data stored in a vertex buffer, enabling efficient rendering
 * by reusing shared vertices to form multiple primitives.
 *
 * This class encapsulates the creation, binding, unbinding, and management of
 * an index buffer in a graphics rendering pipeline. It is generally used in
 * rendering workflows that require indexed drawing, such as rendering
 * triangles, lines, or other primitives.
 */
template <class T> class IndexBuffer final : public BaseBuffer {
public:
  explicit IndexBuffer(VkDeviceSize size);
  void Update(std::vector<T> indices);
};
} // namespace Entropy::Graphics::Vulkan::Buffers

#endif // ENTROPY_VULKAN_INDEX_BUFFER_H
