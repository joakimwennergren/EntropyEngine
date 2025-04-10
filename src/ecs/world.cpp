#include "world.h"

#include "ecs/tags/2d.h"
#include "ecs/components/position.h"
#include "ecs/components/dimension.h"
#include "ecs/components/texture.h"
#include "loggers/logger.h"
#include "renderers/irenderer.h"

using namespace Entropy::ECS;

World::World() {
  const ServiceLocator* sl = ServiceLocator::GetInstance();
  gameWorld_ = std::make_shared<flecs::world>();

 // renderer->instanceData_2d.push_back(
 //   Graphics::Vulkan::Data::InstanceDataTwoD{pos.pos, dim.scale, 0, 0});

  gameWorld_->system<Tags::D2>("2D")
    .each([sl](Tags::D2 &d2) {
      const auto renderer = sl->getService<Renderers::IRenderer>();
      constexpr std::array<Graphics::Vulkan::Data::TwoDVertex, 4> vertices = {{
          {{-1.0f, -1.0f, 0.0f}, {1.0f, 0.0f}},
          {{ 1.0f, -1.0f, 0.0f}, {0.0f, 0.0f}},
          {{ 1.0f,  1.0f, 0.0f}, {0.0f, 1.0f}},
          {{-1.0f,  1.0f, 0.0f}, {1.0f, 1.0f}},
      }};
      constexpr std::array<uint16_t, 6> indices = {{
          0, 1, 2,
          2, 3, 0,
      }};
      renderer->vertices_2d.insert(renderer->vertices_2d.end(), vertices.begin(), vertices.end());
      renderer->indices_2d.insert(renderer->indices_2d.end(), indices.begin(), indices.end());
      d2.id = renderer->objects_2d++;
      //renderer->SwitchPipeline("2D"); @TODO maybe??
  });
  gameWorld_->system<Tags::D2, Components::Position>("Position")
  .each([sl](const Tags::D2 &d2, const Components::Position &pos) {
    const auto renderer = sl->getService<Renderers::IRenderer>();
    renderer->instanceData_2d[d2.id].pos = pos.pos;
  });
  gameWorld_->system<Tags::D2, Components::Dimension>("Dimension")
  .each([sl](const Tags::D2 &d2, const Components::Dimension &dim) {
    const auto renderer = sl->getService<Renderers::IRenderer>();
    renderer->instanceData_2d[d2.id].dim = dim.scale;
  });
  gameWorld_->system<Tags::D2, Components::Texture>("Texture")
  .each([sl](const Tags::D2 &d2, const Components::Texture &tex) {
    const auto renderer = sl->getService<Renderers::IRenderer>();
    const auto region = renderer->assetManager_->textureAtlas.textureRegions[d2.id];
    renderer->instanceData_2d[d2.id].atlasCoords = glm::vec4(region.uMin, region.vMin, region.uMax, region.vMax);
  });
}