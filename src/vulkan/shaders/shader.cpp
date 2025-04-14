#include <fstream>

#include "shader.h"

#include <iostream>

#include "vulkan/utilities/helpers.h"

using namespace Entropy::Vulkan::Shaders;
using namespace Entropy::Vulkan::Devices;

Shader::Shader(const std::string& vert, const std::string& frag) {
  logicalDevice_ = ServiceLocator::GetInstance()->getService<ILogicalDevice>();
  vertCode_ = ReadFile(vert);
  fragCode_ = ReadFile(frag);
  if (!vertCode_.empty() && !fragCode_.empty()) {
    shaderModuleVert_ = BuildShader(vertCode_);
    shaderModuleFrag_ = BuildShader(fragCode_);
  } else {
    LOG_ERROR(logger_, "Could not read shader: {}, {}", vert, frag);
  }
}

Shader::~Shader() {
  vkDestroyShaderModule(logicalDevice_->Get(), shaderModuleVert_, nullptr);
  vkDestroyShaderModule(logicalDevice_->Get(), shaderModuleFrag_, nullptr);
}

std::vector<char> Shader::ReadFile(std::string filename) {
  std::ifstream file(filename, std::ios::ate | std::ios::binary);

  if (!file.is_open()) {
    LOG_INFO(logger_, "Could not read shader: {}", filename);
    return {};
  }

  const std::streamsize fileSize = file.tellg();
  std::vector<char> buffer(fileSize);

  file.seekg(0);
  file.read(buffer.data(), fileSize);

  file.close();

  return buffer;
}

VkShaderModule Shader::BuildShader(const std::vector<char>& code) const {
  VkShaderModuleCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  createInfo.codeSize = code.size();
  createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

  VkShaderModule shaderModule;
  VK_CHECK(vkCreateShaderModule(logicalDevice_->Get(), &createInfo, nullptr,
                                &shaderModule));

  return shaderModule;
}

/*
VkShaderModule Shader::BuildShader(const uint32_t *code, const uint32_t size)
const { VkShaderModuleCreateInfo createInfo{}; createInfo.sType =
VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO; createInfo.codeSize = size;
  createInfo.pCode = code;

  VkShaderModule shaderModule;

  VK_CHECK(vkCreateShaderModule(_logicalDevice->Get(), &createInfo, nullptr,
    &shaderModule));

  return shaderModule;
}
 */
