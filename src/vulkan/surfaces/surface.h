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

#ifndef ENTROPY_VULKAN_SURFACE_H
#define ENTROPY_VULKAN_SURFACE_H

#include "config.h"

#if PLATFORM == MACOS
#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
#endif

#if PLATFORM == LINUX
  #include <vulkan/vulkan.hpp>
  #include <GLFW/glfw3.h>
#endif

#if PLATFORM == WINDOWS
  #define WIN32_LEAN_AND_MEAN
  #include <windows.h>
  #include <vulkan/vulkan_win32.h>
  #include <GLFW/glfw3.h>
  #define GLFW_EXPOSE_NATIVE_WIN32
  #include <GLFW/glfw3native.h>
  #undef max
#endif

#if PLATFORM == IOS
#define VK_USE_PLATFORM_METAL_EXT
#include <vulkan/vulkan.hpp>
#include <MetalKit/MetalKit.hpp>
#endif

#if PLATFORM == ANDROID
#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_android.h>
#endif

#include "vulkan/instances/ivk_instance.h"

namespace Entropy::Graphics::Vulkan::Surfaces {

/**
 * @class Surface
 * @brief Represents a Vulkan surface abstraction for different platforms.
 *
 * The Surface class provides a wrapper for creating and managing Vulkan
 * surfaces based on the platform's specific windowing system.
 */
class Surface {
public:
#if PLATFORM == MACOS || PLATFORM == LINUX || PLATFORM == WINDOWS
  explicit Surface(GLFWwindow *window);
#endif
  //Surface(VkSurfaceKHR surface);
#if PLATFORM == IOS
  explicit Surface(CA::MetalLayer *layer);
#endif
  ~Surface();
  [[nodiscard]] VkSurfaceKHR Get() const { return surface_; };
  VkSurfaceKHR surface_ = VK_NULL_HANDLE;
  std::shared_ptr<Instances::IVulkanInstance> instance_;
};
} // namespace Entropy::Graphics::Vulkan::Surfaces

#endif // ENTROPY_VULKAN_SURFACE_H