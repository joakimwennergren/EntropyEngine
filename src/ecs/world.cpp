#include "world.h"

#include <vulkan/textures/textureatlas.h>

#include <iostream>

#include "ecs/components/asset.h"
#include "ecs/components/dimension.h"
#include "ecs/components/position.h"
#include "ecs/components/rotation.h"
#include "ecs/tags/2d.h"
#include "renderers/irenderer.h"

using namespace Entropy::ECS;
using namespace Entropy::Graphics::Vulkan::Data;

World::World() {
  sl_ = ServiceLocator::GetInstance();
  gameWorld_ = std::make_shared<flecs::world>();
  gameWorld_->system<Tags::D2>("2D")
    .each([this](Tags::D2 &d2) {
      const auto renderer = sl_->getService<Renderers::IRenderer>();
      frame_.vertices.push_back({{-1.0f, -1.0f, 0.0f}, {1.0f, 0.0f}});
      frame_.vertices.push_back({{ 1.0f, -1.0f, 0.0f}, {0.0f, 0.0f}});
      frame_.vertices.push_back({{ 1.0f,  1.0f, 0.0f}, {0.0f, 1.0f}});
      frame_.vertices.push_back({{-1.0f,  1.0f, 0.0f}, {1.0f, 1.0f}});
      frame_.indices = {0, 1, 2, 2, 3, 0};
      frame_.data.push_back({glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f)});
      d2.index = renderer->Frame(&frame_);
  });

  gameWorld_->system<Tags::D2, Components::Position>("Position")
  .each([this](const Tags::D2 d2, const Components::Position &pos) {
    const auto renderer = sl_->getService<Renderers::IRenderer>();
    renderer->GetFrame()->data[d2.index - 1].pos = glm::vec3{pos.x, pos.y, pos.z};
  });

  gameWorld_->system<Tags::D2, Components::Dimension>("Dimension")
  .each([this](const Tags::D2 &d2, const Components::Dimension &dim) {
    const auto renderer = sl_->getService<Renderers::IRenderer>();
    renderer->GetFrame()->data[d2.index - 1].dim = glm::vec2{dim.x, dim.y};
  });

  gameWorld_->system<Tags::D2, Components::Rotation>("Rotation")
  .each([this](const Tags::D2 &d2, const Components::Rotation &rot) {
    const auto renderer = sl_->getService<Renderers::IRenderer>();
    renderer->GetFrame()->data[d2.index - 1].rot = glm::radians(rot.angle);
  });

  gameWorld_->system<Tags::D2, Components::Asset>("Texture")
  .each([this](const Tags::D2 &d2, const Components::Asset &asset) {
    static uint32_t old_atlas_size;
    const auto renderer = sl_->getService<Renderers::IRenderer>();
    const auto logicalDevice = sl_->getService<Graphics::Vulkan::Devices::ILogicalDevice>();
    const auto atlas = static_cast<Graphics::Vulkan::Textures::TextureAtlas*>(asset.handles[0].asset);
    if (atlas->image_paths.size() != old_atlas_size) {
      VkWriteDescriptorSet descriptorWrite{};
      VkDescriptorImageInfo imageInfo;
      imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
      imageInfo.imageView = atlas->texture_->GetImageView()->Get();
      imageInfo.sampler = atlas->texture_->GetSampler();
      descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
      descriptorWrite.dstSet = renderer->two_d_pipeline_->descriptorSet;
      descriptorWrite.dstBinding = 2;
      descriptorWrite.dstArrayElement = 0;
      descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
      descriptorWrite.descriptorCount = 1;
      descriptorWrite.pImageInfo = &imageInfo;
      vkUpdateDescriptorSets(logicalDevice->Get(), 1, &descriptorWrite, 0, nullptr);
      LOG_WARNING(logger_, "Updating texture atlas descriptor set");
    }
    const auto regionIndex = std::distance( atlas->image_paths.begin(), std::find( atlas->image_paths.begin(), atlas->image_paths.end(), asset.handles[0].path));
    auto [uMin, vMin, uMax, vMax] = atlas->textureRegions[regionIndex];
    renderer->GetFrame()->data[d2.index - 1].atlasCoords = glm::vec4(uMin, vMin, uMax, vMax);
    old_atlas_size = atlas->image_paths.size();
  });
}