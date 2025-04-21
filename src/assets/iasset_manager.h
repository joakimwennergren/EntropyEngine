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

#include <functional>
#include <future>
#include <mutex>
#include <string>
#include <unordered_map>
#include "iasset.h"
#include "servicelocators/servicelocator.h"

namespace Entropy::Assets {
class IAssetManager : public IService {
 public:
  template <typename T>
  using LoaderFunc = std::function<std::shared_ptr<T>(const std::string& path)>;

  template <typename T>
  void RegisterLoader(const LoaderFunc<T>& loader) {
    std::lock_guard<std::mutex> lock(mutex_);
    loaders_[typeid(T).hash_code()] =
        [loader](const std::string& path) -> std::shared_ptr<IAsset> {
      return loader(path);
    };
  }

  template <typename T>
  std::shared_ptr<T> Load(const char* name, const char* path) {
    std::lock_guard<std::mutex> lock(mutex_);
    auto& map = assets_[typeid(T).hash_code()];
    auto it = map.find(std::string(name));
    if (it != map.end()) {
      return std::static_pointer_cast<T>(it->second);
    }

    auto loaderIt = loaders_.find(typeid(T).hash_code());
    if (loaderIt == loaders_.end()) {
      throw std::runtime_error("No loader registered for this asset type");
    }

    auto asset =
        std::static_pointer_cast<T>(loaderIt->second(std::string(path)));
    map[name] = asset;
    return asset;
  }

  template <typename T>
  std::future<std::shared_ptr<T>> LoadAsync(const std::string& name,
                                            const std::string& path) {
    return std::async(std::launch::async,
                      [=]() { return this->Load<T>(name, path); });
  }

  template <typename T>
  std::shared_ptr<T> Get(const std::string& name) {
    std::lock_guard<std::mutex> lock(mutex_);
    auto& map = assets_[typeid(T).hash_code()];
    auto it = map.find(name);
    if (it != map.end()) {
      return std::static_pointer_cast<T>(it->second);
    }
    return nullptr;
  }

 private:
  std::unordered_map<size_t,
                     std::function<std::shared_ptr<IAsset>(const std::string&)>>
      loaders_;
  std::unordered_map<size_t,
                     std::unordered_map<std::string, std::shared_ptr<IAsset>>>
      assets_;
  std::mutex mutex_;
};
}  // namespace Entropy::Assets
#endif  // ENTROPY_ASSETS_IASSET_MANAGER_H
