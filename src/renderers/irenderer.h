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

#ifndef ENTROPY_RENDERERS_IRENDERER_H
#define ENTROPY_RENDERERS_IRENDERER_H

#include "servicelocators/servicelocator.h"
#include "vulkan/data/vertex.h"

#include <assets/iasset_manager.h>

namespace Entropy::Renderers {
class IRenderer : public IService {
public:
  ~IRenderer() override = default;
  virtual void Render(uint32_t width, uint32_t height) = 0;
  virtual void Resize(uint32_t width, uint32_t height) = 0;
  std::vector<Graphics::Vulkan::Data::TwoDVertex> vertices_2d;
  std::vector<uint16_t> indices_2d;
  std::vector<Graphics::Vulkan::Data::InstanceDataTwoD> instanceData_2d;
  uint32_t objects_2d;
  std::shared_ptr<Assets::IAssetManager> assetManager_;
};

} // namespace Entropy::Graphics::Vulkan::CommandPools

#endif // ENTROPY_RENDERERS_IRENDERER_H
