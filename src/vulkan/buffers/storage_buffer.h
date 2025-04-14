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

#ifndef ENTROPY_VULKAN_STORAGE_BUFFER_H
#define ENTROPY_VULKAN_STORAGE_BUFFER_H

#include "base_buffer.h"

namespace Entropy::Vulkan::Buffers {
/**
 * @class StorageBuffer
 *
 * @brief A class for managing a storage buffer used for data storage and
 * retrieval.
 *
 * The StorageBuffer class provides an efficient and flexible way to manage
 * data in a storage buffer. It supports operations such as allocation,
 * deallocation, data access, and updating the buffer contents dynamically.
 * This class is commonly utilized in applications requiring buffer management
 * such as graphics programming, data processing, and other performance-critical
 * computations.
 *
 * Features:
 * - Allocates and maintains a buffer in memory.
 * - Provides methods to access, update, and manage buffer contents.
 * - Ensures efficient memory usage with options for resizing or releasing the
 * buffer.
 *
 * Thread Safety:
 * - The thread safety of this class depends on the implementation.
 * - Ensure proper synchronization when accessing the buffer in multithreaded
 * environments.
 *
 * Usage Notes:
 * - The size and alignment constraints of the buffer may depend on the
 *   underlying implementation or hardware.
 * - Proper initialization and cleanup of the buffer is recommended to avoid
 *   memory leaks or undefined behavior.
 *
 * Exception Safety:
 * - If any methods throw exceptions, ensure proper exception handling
 *   to maintain a consistent state.
 */
class StorageBuffer final : public BaseBuffer {
 public:
  explicit StorageBuffer(VkDeviceSize bufferSize);
};
}  // namespace Entropy::Vulkan::Buffers

#endif  // ENTROPY_VULKAN_STORAGE_BUFFER_H
