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

#ifndef ENTROPY_VULKAN_VALIDATION_LAYER_H
#define ENTROPY_VULKAN_VALIDATION_LAYER_H

#include <vulkan/vulkan.hpp>
#include "loggers/logger.h"

namespace Entropy::Vulkan::ValidationLayers {

class ValidationLayer final {
 public:
  /**
 * @brief Checks if the required validation layers are supported by the Vulkan instance.
 * 
 * @param validationLayers A vector of validation layer names to check for support.
 * @return True if all specified validation layers are supported, false otherwise.
 */
  static bool CheckValidationLayerSupport(
      const std::vector<const char*>& validationLayers);

  /**
 * @brief Creates a Vulkan debug messenger for handling debug messages.
 * 
 * @param instance The Vulkan instance to associate the debug messenger with.
 * @param pCreateInfo A pointer to the debug messenger creation info structure.
 * @param pAllocator Optional allocation callbacks for custom memory management.
 * @param pDebugMessenger A pointer to the created debug messenger handle.
 * @return VkResult indicating success or failure of the operation.
 */
  static VkResult CreateDebugUtilsMessengerEXT(
      VkInstance instance,
      const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
      const VkAllocationCallbacks* pAllocator,
      VkDebugUtilsMessengerEXT* pDebugMessenger);

  /**
 * @brief Destroys a Vulkan debug messenger.
 * 
 * @param instance The Vulkan instance associated with the debug messenger.
 * @param debugMessenger The debug messenger to destroy.
 * @param pAllocator Optional allocation callbacks for custom memory management.
 */
  static void DestroyDebugUtilsMessengerEXT(
      VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger,
      const VkAllocationCallbacks* pAllocator);

  /**
 * @brief Callback function for handling Vulkan debug messages.
 * 
 * @param messageSeverity The severity of the debug message.
 * @param messageType The type of the debug message.
 * @param pCallbackData A pointer to the callback data containing message details.
 * @param pUserData A pointer to user-defined data passed to the callback.
 * @return VkBool32 indicating whether the Vulkan call that triggered the callback should be aborted.
 */
  static VkBool32 VKAPI_CALL
  debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                VkDebugUtilsMessageTypeFlagsEXT messageType,
                const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                void* pUserData);

  VkDebugUtilsMessengerEXT debugMessenger = VK_NULL_HANDLE;
};
}  // namespace Entropy::Vulkan::ValidationLayers

#endif  // ENTROPY_VULKAN_VALIDATION_LAYER_H
