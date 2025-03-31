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

#ifndef ENTROPY_SERVICE_LOCATOR_H
#define ENTROPY_SERVICE_LOCATOR_H

#include <map>
#include <mutex>
#include <stdexcept>
#include <typeindex>

class IService {
public:
  virtual ~IService() = default;
  [[nodiscard]] virtual std::type_index getTypeIndex() const = 0;
};

template <typename T> class ServiceBase : public T {
public:
  [[nodiscard]] std::type_index getTypeIndex() const override {
    return std::type_index(typeid(T));
  }
};

class ServiceLocator {
public:
  std::map<std::type_index, std::shared_ptr<IService>> services;
  mutable std::mutex mutex;

protected:
  ServiceLocator() {}
  static ServiceLocator *_instance;

public:
  ServiceLocator(ServiceLocator &other) = delete;
  void operator=(const ServiceLocator &) = delete;
  static ServiceLocator *GetInstance();

  template <typename T> void registerService(std::shared_ptr<T> service) {
    static_assert(std::is_base_of_v<IService, T>,
                  "T must inherit from IService");
    std::lock_guard lock(mutex);
    const std::type_index typeIndex =
        service->getTypeIndex(); // Use base class type index
    if (services.find(typeIndex) != services.end()) {
      throw std::runtime_error("Service already registered");
    }
    services[typeIndex] = service;
  }

  template <typename T> void unregisterService() {
    std::lock_guard lock(mutex);
    const auto typeIndex = std::type_index(typeid(T));
    if (services.find(typeIndex) == services.end()) {
      throw std::runtime_error("Service not registered");
    }
    services.erase(typeIndex);
  }

  template <typename T> std::shared_ptr<T> getService() const {
    std::lock_guard lock(mutex);
    const auto typeIndex = std::type_index(typeid(T));
    const auto it = services.find(typeIndex);
    if (it == services.end()) {
      throw std::runtime_error("Service not found");
    }
    return std::static_pointer_cast<T>(it->second);
  }
};

#endif // ENTROPY_SERVICE_LOCATOR_H
