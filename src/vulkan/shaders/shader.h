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

#ifndef ENTROPY_VULKAN_SHADER_H
#define ENTROPY_VULKAN_SHADER_H

#include <vulkan/vulkan.hpp>
#include "vulkan/devices/ilogical_device.h"

namespace Entropy::Graphics::Vulkan::Shaders {

/**
 * @class Shader
 * @brief The Shader class is used for managing and handling GPU shaders.
 *
 * This class provides functionality to load, compile, link, and manage
 * vertex and fragment shaders for rendering graphics in OpenGL or similar
 * graphics APIs. It encapsulates the creation and binding of shader programs,
 * allowing users to work with shaders efficiently.
 *
 * The Shader class supports the compilation of individual shader components,
 * linking them into a complete shader program, and setting uniform values
 * for use during rendering. It is an essential utility for graphics
 * applications requiring custom shader functionality.
 */
class Shader {
public:
  Shader(const std::string &vert, const std::string &frag);
  ~Shader();

  [[nodiscard]] VkShaderModule GetVertShaderModule() const {
    return shaderModuleVert_;
  }
  [[nodiscard]] VkShaderModule GetFragShaderModule() const {
    return shaderModuleFrag_;
  }

private:
  static std::vector<char> ReadFile(std::string filename);
  [[nodiscard]] VkShaderModule BuildShader(const std::vector<char> &code) const;
  // @TODO find another way for android..
  // VkShaderModule BuildShader(const uint32_t *code, uint32_t size) const;

  std::vector<char> vertCode_;
  std::vector<char> fragCode_;
  VkShaderModule shaderModuleVert_ = VK_NULL_HANDLE;
  VkShaderModule shaderModuleFrag_ = VK_NULL_HANDLE;
  std::shared_ptr<Devices::ILogicalDevice> logicalDevice_;
};
} // namespace Entropy::Graphics::Vulkan::Shaders

#endif // ENTROPY_VULKAN_SHADER_H
