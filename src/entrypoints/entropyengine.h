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

#ifndef ENTROPY_ENTRYPOINTS_ENTROPY_H
#define ENTROPY_ENTRYPOINTS_ENTROPY_H

#include "renderers/vulkan_renderer.h"
#include "config.h"

namespace Entropy::EntryPoints {

/**
 * @class EntropyEngine
 *
 * @brief A class that serves as the core engine for initializing and managing the rendering infrastructure
 *        using Vulkan. It handles registration and un registration of various Vulkan-related services.
 *
 * The EntropyEngine is platform-dependent, with constructors tailored for different operating systems.
 * It initializes Vulkan services and manages a Vulkan renderer instance.
 */
class EntropyEngine {
 public:
  void Run() const;
  ~EntropyEngine();
#if ENTROPY_PLATFORM == IOS
  EntropyEngine(void* layer, uint32_t width, uint32_t height);
#elif ENTROPY_PLATFORM == MACOS || ENTROPY_PLATFORM == LINUX
  EntropyEngine(uint32_t width, uint32_t height);
private:
  static void OnFramebufferResize(GLFWwindow* window, int width, int height);
  GLFWwindow* window_;
#endif
private:
  static void RegisterServices();
  static void UnRegisterServices();
};
}  // namespace Entropy::EntryPoints

#endif  // ENTROPY_ENTRYPOINTS_ENTROPY_H
