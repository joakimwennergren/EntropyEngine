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

#include <vulkan/vulkan.h>
#include "config.h"

#if ENTROPY_PLATFORM == MACOS
#include <GLFW/glfw3.h>

#elif ENTROPY_PLATFORM == LINUX
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>

#elif ENTROPY_PLATFORM == WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <vulkan/vulkan_win32.h>
#undef max

#elif ENTROPY_PLATFORM == IOS
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_metal.h>

#elif ENTROPY_PLATFORM == ANDROID
#include <vulkan/vulkan_android.h>
#include <vulkan/vulkan.hpp>

#else
#include <vulkan/vulkan.hpp>
#endif

#include "vulkan/instances/ivk_instance.h"

namespace Entropy::Vulkan::Surfaces {

/**
 * @class Surface
 * @brief Represents a Vulkan surface abstraction for different platforms.
 *
 * The Surface class provides a wrapper for creating and managing Vulkan
 * surfaces based on the platform's specific windowing system.
 */
class Surface {
 public:
#if ENTROPY_PLATFORM == MACOS
  explicit Surface(GLFWwindow* window);
#elif ENTROPY_PLATFORM == LINUX
  explicit Surface(GLFWwindow* window);
#elif ENTROPY_PLATFORM == IOS
  explicit Surface(CAMetalLayer* layer);
#else
  explicit Surface(VkSurfaceKHR surface);
#endif
  ~Surface();
  [[nodiscard]] VkSurfaceKHR Get() const { return surface_; };
  VkSurfaceKHR surface_ = VK_NULL_HANDLE;
  std::shared_ptr<Instances::IVulkanInstance> instance_;
};
}  // namespace Entropy::Vulkan::Surfaces

#endif  // ENTROPY_VULKAN_SURFACE_H
