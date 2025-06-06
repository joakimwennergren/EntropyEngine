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

#ifndef ENTROPY_VULKAN_TEXTURE_H
#define ENTROPY_VULKAN_TEXTURE_H

#include "base_texture.h"

namespace Entropy::Vulkan::Textures {
class Texture final : public BaseTexture {
 public:
  explicit Texture() = default;
  explicit Texture(int32_t width, int32_t height);
  explicit Texture(const std::vector<uint8_t>& data, int width, int height);
  explicit Texture(const std::string& path);
  explicit Texture(std::vector<std::string>& paths, uint32_t targetWidth,
                   uint32_t targetHeight);
  static std::vector<uint8_t> LoadAndResize(const std::string& path,
                                            uint32_t targetWidth,
                                            uint32_t targetHeight);
  // Texture(const FT_Bitmap &bitmap);
 private:
  void Create(int width, int height);
  void CreateTextureSampler();
  std::vector<std::string> loaded_paths_;
};
}  // namespace Entropy::Vulkan::Textures

#endif  // ENTROPY_VULKAN_TEXTURE_H
