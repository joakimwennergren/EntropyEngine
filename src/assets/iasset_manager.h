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

#ifndef ENTROPY_ASSETS_IASSET_MANAGER_H
#define ENTROPY_ASSETS_IASSET_MANAGER_H

#include "servicelocators/servicelocator.h"
#include "vulkan/textures/texture.h"

namespace Entropy::Assets {
  class IAssetManager : public IService {
  public:
    ~IAssetManager() override = default;

    template <typename T>
    int32_t Load(const std::string& path) {
      return LoadImpl<T>(path);
    }

    template <typename T>
    int32_t LoadAsync(const std::string& path) {
      return LoadAsyncImpl<T>(path);
    }

    template <typename T>
    void Unload(const std::string& path) {
      UnloadImpl<T>(path);
    }

   template <typename T>
    std::shared_ptr<T> Get(const std::string& path) {
        return GetImpl<T>(path);
    }

    template <typename T>
     std::shared_ptr<T> GetAsync(const std::string& path) {
          return GetAsyncImpl<T>(path);
        }

  protected:
    virtual int32_t LoadTexture(const std::string& path) = 0;
    virtual int32_t LoadTextureAsync(const std::string& path) = 0;
    virtual std::shared_ptr<Graphics::Vulkan::Textures::Texture> GetTexture(const std::string& path) = 0;
    virtual std::shared_ptr<Graphics::Vulkan::Textures::Texture> GetTextureAsync(const std::string& path) = 0;
    virtual void UnloadTexture(const std::string& path) = 0;

  private:
    template <typename T>
    int32_t LoadImpl(const std::string& path);

    template <typename T>
    int32_t LoadAsyncImpl(const std::string& path);

    template <typename T>
    std::shared_ptr<T> GetImpl(const std::string& path);

    template <typename T>
    std::shared_ptr<T> GetAsyncImpl(const std::string& path);

    template <typename T>
    void UnloadImpl(const std::string& path);
  };

  // Template Specialization in Interface
  template <>
  inline int32_t IAssetManager::LoadImpl<Graphics::Vulkan::Textures::Texture>(const std::string& path) {
    return LoadTexture(path);
  }

  // Template Specialization in Interface
  template <>
  inline int32_t IAssetManager::LoadAsyncImpl<Graphics::Vulkan::Textures::Texture>(const std::string& path) {
    return LoadTextureAsync(path);
  }

  template <>
  inline std::shared_ptr<Graphics::Vulkan::Textures::Texture> IAssetManager::GetImpl<Graphics::Vulkan::Textures::Texture>(const std::string& path) {
    return GetTexture(path);
  }

  template <>
  inline std::shared_ptr<Graphics::Vulkan::Textures::Texture> IAssetManager::GetAsyncImpl<Graphics::Vulkan::Textures::Texture>(const std::string& path) {
    return GetTextureAsync(path);
  }

  template <>
  inline void IAssetManager::UnloadImpl<Graphics::Vulkan::Textures::Texture>(const std::string& path) {
    UnloadTexture(path);
  }
}

#endif // ENTROPY_ASSETS_IASSET_MANAGER_H
