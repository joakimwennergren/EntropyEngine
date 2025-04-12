#include "world.h"

#include <vulkan/textures/textureatlas.h>

#include <iostream>

#include "ecs/components/dimension.h"
#include "ecs/components/position.h"
#include "ecs/components/asset.h"
#include "ecs/tags/2d.h"
#include "ecs/tags/textureatlas.h"
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
    renderer->GetFrame()->data[d2.index - 1].pos = pos.pos;
  });

  gameWorld_->system<Tags::D2, Components::Dimension>("Dimension")
  .each([this](const Tags::D2 &d2, const Components::Dimension &dim) {
    const auto renderer = sl_->getService<Renderers::IRenderer>();
    renderer->GetFrame()->data[d2.index - 1].dim = dim.scale;
  });

  // @TODO this is only experimental..!!
  // Dont update every frame and stuff...
  gameWorld_->system<Tags::D2, Tags::TextureAtlas, Components::Asset>("Texture")
  .each([this](const Tags::D2 &d2, const Tags::TextureAtlas &atlas_t, const Components::Asset &asset) {

    const auto renderer = sl_->getService<Renderers::IRenderer>();
    const auto logicalDevice = sl_->getService<Graphics::Vulkan::Devices::ILogicalDevice>();
    const auto atlas = static_cast<Graphics::Vulkan::Textures::TextureAtlas*>(asset.handles[0].asset);
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
    vkUpdateDescriptorSets(logicalDevice->Get(), 1,&descriptorWrite, 0, nullptr);
    auto [uMin, vMin, uMax, vMax] = atlas->textureRegions[atlas_t.region_index];
    renderer->GetFrame()->data[d2.index - 1].atlasCoords = glm::vec4(uMin, vMin, uMax, vMax);
  });
}