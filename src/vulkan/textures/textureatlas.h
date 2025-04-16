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

#ifndef ENTROPY_VULKAN_TEXTURES_ATLAS_H
#define ENTROPY_VULKAN_TEXTURES_ATLAS_H

#include "texture.h"

namespace Entropy::Vulkan::Textures {
/**
 * @class TextureAtlas
 * @brief Represents a texture atlas that combines multiple textures into a single texture.
 * 
 * The TextureAtlas class is responsible for loading multiple textures from file paths,
 * combining them into a single texture, and providing texture regions for each individual
 * texture within the atlas. This is useful for optimizing rendering by reducing the number
 * of texture bindings.
 * 
 * @note This class is marked as `final`, meaning it cannot be inherited.
 * 
 * @details
 * - The `TextureAtlas` constructor takes a vector of file paths to load textures.
 * - The `Create` method generates the texture atlas from the provided textures.
 * - The `Save` method allows saving the generated texture atlas to a file.
 * - The `TextureRegion` struct defines the UV coordinates for each texture in the atlas.
 * 
 * @example
 * ```
 * std::vector<std::string> texturePaths = {"texture1.png", "texture2.png"};
 * TextureAtlas atlas(texturePaths);
 * if (atlas.Create()) {
 *     atlas.Save("atlas.png");
 * }
 * ```
 */
class TextureAtlas final {
 public:
  TextureAtlas(std::vector<std::string>& paths);
  struct TextureRegion {
    float uMin{};
    float vMin{};
    float uMax{};
    float vMax{};
  };
  /**
   * @brief Creates and initializes the texture atlas.
   * 
   * This function is responsible for setting up the texture atlas,
   * allocating necessary resources, and preparing it for use in
   * rendering operations. It ensures that the texture atlas is
   * properly initialized before being accessed.
   * 
   * @return true if the texture atlas was successfully created and initialized.
   * @return false if there was an error during the creation process.
   */
  bool Create();
  /**
   * @brief Saves the texture atlas to a file.
   * 
   * This function writes the texture atlas data to a file with the specified name.
   * The file can later be used to reload the texture atlas.
   * 
   * @param name The name of the file to save the texture atlas to.
   */
  void Save(const std::string& name) const;

  std::shared_ptr<Texture> texture;
  std::vector<TextureRegion> regions;

 private:
  std::vector<std::string> paths_;
  std::vector<uint8_t> atlas_;
  int width_, height_;
};
}  // namespace Entropy::Vulkan::Textures

#endif  // ENTROPY_VULKAN_TEXTURES_ATLAS_H
