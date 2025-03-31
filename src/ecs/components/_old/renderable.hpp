#pragma once

#include <cstdint>
#include <flecs.h>

#include "graphics/vulkan/buffers/indexbuffer.hpp"
#include "graphics/vulkan/buffers/vertexbuffer.hpp"

namespace Entropy::Components {
struct Renderable {
  uint32_t id = 0;
  bool visible = true;
  std::shared_ptr<VertexBuffer> vertexBuffer = nullptr;
  std::shared_ptr<IndexBuffer<uint16_t> > indexBuffer = nullptr;
  std::vector<uint16_t> indices = {};
  std::vector<Vertex> vertices = {};
  int zIndex = 0;
  int type = 0;
  std::vector<flecs::entity> children;
};
}  // namespace Entropy::Components
