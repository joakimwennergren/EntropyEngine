#include "world.h"

#include <vulkan/textures/textureatlas.h>
#include "ecs/components/asset.h"
#include "ecs/components/color.h"
#include "ecs/components/dimension.h"
#include "ecs/components/position.h"
#include "ecs/components/rotation.h"
#include "ecs/components/texture.h"
#include "ecs/tags/2d.h"

using namespace Entropy::ECS;
using namespace Entropy::Vulkan::Data;

World::World() {
  renderer_ = ServiceLocator::GetInstance()->GetService<Renderers::IRenderer>();
  logicalDevice_ = ServiceLocator::GetInstance()
                       ->GetService<Vulkan::Devices::ILogicalDevice>();
  assetManager_ =
      ServiceLocator::GetInstance()->GetService<Assets::IAssetManager>();
  gameWorld_ = std::make_shared<flecs::world>();

  gameWorld_->observer<Tags::D2>().event(flecs::OnSet).each([&](Tags::D2 d2) {
    auto& entityData = renderer_->entityData;
    entityData.vertices.push_back({{-1.0f, -1.0f, 0.0f}, {1.0f, 0.0f}});
    entityData.vertices.push_back({{1.0f, -1.0f, 0.0f}, {0.0f, 0.0f}});
    entityData.vertices.push_back({{1.0f, 1.0f, 0.0f}, {0.0f, 1.0f}});
    entityData.vertices.push_back({{-1.0f, 1.0f, 0.0f}, {1.0f, 1.0f}});
    entityData.indices = {0, 1, 2, 2, 3, 0};
    entityData.data.emplace_back(InstanceDataTwoD{
        .pos = {0.0f, 0.0f},
        .dim = {0.0f, 0.0f},
        .rot = 0.0f,
        .texture_index = -1.0f,
        .atlasCoords = {0.0f, 0.0f, 0.0f, 0.0f},
        .color = {1.0f, 1.0f, 1.0f, 1.0f},
    });
  });

  gameWorld_->system<Tags::D2, Components::Position>("2D::Position")
      .each([&](const Tags::D2 d2, const Components::Position& pos) {
        static uint32_t index;
        renderer_->entityData.data[index].pos = glm::vec3{pos.x, pos.y, pos.z};
        index = (index + 1) % renderer_->entityData.data.size();
      });

  gameWorld_->system<Tags::D2, Components::Dimension>("2D::Dimension")
      .each([&](const Tags::D2 d2, const Components::Dimension& dim) {
        static uint32_t index;
        renderer_->entityData.data[index].dim = glm::vec2{dim.x, dim.y};
        index = (index + 1) % renderer_->entityData.data.size();
      });

  gameWorld_->system<Tags::D2, Components::Rotation>("2D::Rotation")
      .each([&](const Tags::D2 d2, const Components::Rotation& rot) {
        static uint32_t index;
        renderer_->entityData.data[index].rot = glm::radians(rot.angle);
        index = (index + 1) % renderer_->entityData.data.size();
      });

  gameWorld_->system<Tags::D2, Components::Color>("2D::Color")
      .each([&](const Tags::D2 d2, const Components::Color& col) {
        static uint32_t index;
        renderer_->entityData.data[index].color =
            glm::vec4{col.r, col.g, col.b, col.a};
        index = (index + 1) % renderer_->entityData.data.size();
      });

  gameWorld_->system<Tags::D2>("2D::Texture")
      .each([this](flecs::entity e, const Tags::D2& d2) {
        static uint32_t entity_index;
        if (e.has<Components::TextureComponent>()) {
          auto tex_comp = e.get_ref<const Components::TextureComponent>();
          auto tex = assetManager_->Get<Assets::TextureAsset>(tex_comp->name);
          if (tex) {
            VkWriteDescriptorSet descriptorWrite{};
            VkDescriptorImageInfo imageInfo;
            imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            imageInfo.imageView = tex->texture->GetImageView()->Get();
            imageInfo.sampler = tex->texture->GetSampler();
            descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrite.dstSet = renderer_->two_d_pipeline_->descriptorSet;
            descriptorWrite.dstBinding = 2;
            descriptorWrite.dstArrayElement = 0;
            descriptorWrite.descriptorType =
                VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            descriptorWrite.descriptorCount = 1;
            descriptorWrite.pImageInfo = &imageInfo;
            vkUpdateDescriptorSets(logicalDevice_->Get(), 1, &descriptorWrite,
                                   0, nullptr);
            renderer_->entityData.data[entity_index].texture_index =
                tex_comp->index;
          } else {
            renderer_->entityData.data[entity_index].texture_index = -1.0f;
          }
        }

        entity_index = (entity_index + 1) % renderer_->entityData.data.size();

        /*
        static uint32_t old_atlas_size;
        const auto renderer = sl_->GetService<Renderers::IRenderer>();
        const auto logicalDevice =
            sl_->GetService<Vulkan::Devices::ILogicalDevice>();
        const auto atlas = static_cast<Vulkan::Textures::TextureAtlas*>(
            asset.handles[0].asset);
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
          descriptorWrite.descriptorType =
              VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
          descriptorWrite.descriptorCount = 1;
          descriptorWrite.pImageInfo = &imageInfo;
          vkUpdateDescriptorSets(logicalDevice->Get(), 1, &descriptorWrite, 0,
                                 nullptr);
          LOG_WARNING(logger_, "Updating texture atlas descriptor set");
        }
        const auto regionIndex = std::distance(
            atlas->image_paths.begin(),
            std::find(atlas->image_paths.begin(), atlas->image_paths.end(),
                      asset.handles[0].path));
        auto [uMin, vMin, uMax, vMax] = atlas->textureRegions[regionIndex];
        renderer->GetFrame()->data[d2.index - 1].atlasCoords =
            glm::vec4(uMin, vMin, uMax, vMax);
        old_atlas_size = atlas->image_paths.size();
        */
      });
}