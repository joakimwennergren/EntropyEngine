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

#ifndef ENTROPY_VULKAN_UNIFORM_BUFFER_H
#define ENTROPY_VULKAN_UNIFORM_BUFFER_H

#include "base_buffer.h"

namespace Entropy::Graphics::Vulkan::Buffers {
/**
 * @class UniformBuffer
 * @brief Represents a GPU uniform buffer object used for efficiently updating
 * and managing uniform variables shared across shaders.
 *
 * The UniformBuffer class provides an abstraction for creating, binding,
 * and updating uniform buffers to send uniform data to the shaders.
 * Uniform buffers store data once and allow multiple shaders to utilize
 * the same buffer, improving performance and reducing redundant operations.
 *
 * The class facilitates managing GPU buffer memory, providing methods
 * to allocate, update, and bind data to the buffer objects. It is typically
 * used in graphics applications such as rendering pipelines.
 *
 * Key features include:
 * - Management of buffer data updates without frequent re-uploading
 *   to GPU memory.
 * - Compatibility with graphics APIs that support uniform buffer objects.
 * - Simplification of memory management for frequently used uniform variables.
 *
 * Users of this class should ensure that it is properly initialized,
 * updated with the correct size, and bound to the shader pipeline
 * before use.
 */
class UniformBuffer final : public BaseBuffer {
public:
  explicit UniformBuffer(VkDeviceSize bufferSize);
};
} // namespace Entropy::Graphics::Vulkan::Buffers

#endif // ENTROPY_VULKAN_UNIFORM_BUFFER_H
