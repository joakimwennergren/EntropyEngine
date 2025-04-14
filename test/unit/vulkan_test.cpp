#include <gtest/gtest.h>

// Renderers
#include "renderers/vulkan_renderer.h"

// sync
#include "vulkan/synchronization/synchronizer.h"

// CameraManager
#include "cameras/camera_manager.h"
#include "cameras/icamera_manger.h"

// TextureManager
#include "assets/asset_manager.h"
#include "assets/iasset_manager.h"

// ECS
#include "ecs/components/2d_quad.h"
#include "ecs/components/dimension.h"
#include "ecs/components/position.h"
#include "ecs/iworld.h"
#include "ecs/world.h"

// Pipelines
#include "vulkan/pipelines/twod_pipeline.h"

// RenderPass
#include "vulkan/renderpasses/renderpass.h"

// SwapChain
#include "vulkan/swapchains/iswapchain.h"
#include "vulkan/swapchains/swapchain.h"

// Surfaces
#include "vulkan/surfaces/surface.h"

// Textures
#include "vulkan/textures/depthbuffer_texture.h"
#include "vulkan/textures/swapchain_texture.h"
#include "vulkan/textures/texture.h"

// Shader
#include "vulkan/shaders/shader.h"

// PipelineCache
#include "vulkan/pipelinecaches/ipipeline_cache.h"
#include "vulkan/pipelinecaches/pipeline_cache.h"

// DescriptorSet
#include "vulkan/descriptorsets/descriptorset.h"

// DescriptorSetLayout
#include "vulkan/descriptorsetlayouts/descriptorset_layout.h"

// DescriptorPools
#include "vulkan/descriptorpools/descriptorpool.h"
#include "vulkan/descriptorpools/idescriptorpool.h"

// CommandBuffer
#include "vulkan/commandbuffers/commandbuffer.h"

// CommandPool
#include "vulkan/commandpools/commandpool.h"
#include "vulkan/commandpools/icommandpool.h"

// Buffers
#include "vulkan/buffers/base_buffer.h"
#include "vulkan/buffers/index_buffer.h"
#include "vulkan/buffers/staging_buffer.h"
#include "vulkan/buffers/storage_buffer.h"
#include "vulkan/buffers/uniform_buffer.h"
#include "vulkan/buffers/vertex_buffer.h"

// Memory Allocator
#include "vulkan/memory/allocator.h"
#include "vulkan/memory/iallocator.h"

// Vulkan Devices
#include "vulkan/devices/ilogical_device.h"
#include "vulkan/devices/iphysical_device.h"
#include "vulkan/devices/logical_device.h"
#include "vulkan/devices/physical_device.h"

// Vulkan Instance
#include "vulkan/instances/ivk_instance.h"
#include "vulkan/instances/vk_instance.h"

#include "loggers/logger.h"
#include "servicelocators/servicelocator.h"

#include "config.h"

using namespace Entropy::Vulkan;

class Dut : public ::testing::Test {
 protected:
  void SetUp() override {
    InitializeQuill();
    ServiceLocator* sl = ServiceLocator::GetInstance();
    sl->RegisterService(std::make_shared<::Instances::VulkanInstance>());
    sl->RegisterService(std::make_shared<::Devices::PhysicalDevice>());
    sl->RegisterService(std::make_shared<::Devices::LogicalDevice>());
    sl->RegisterService(std::make_shared<::Memory::Allocator>());
    sl->RegisterService(std::make_shared<::CommandPools::CommandPool>());
    sl->RegisterService(std::make_shared<::DescriptorPools::DescriptorPool>());
    sl->RegisterService(std::make_shared<::Caches::PipelineCache>());
    sl->RegisterService(std::make_shared<::SwapChains::SwapChain>());
  }

  void TearDown() override {
    ServiceLocator* sl = ServiceLocator::GetInstance();
    sl->UnregisterService<::SwapChains::ISwapChain>();
    sl->UnregisterService<::Caches::IPipelineCache>();
    sl->UnregisterService<::DescriptorPools::IDescriptorPool>();
    sl->UnregisterService<::CommandPools::ICommandPool>();
    sl->UnregisterService<::Memory::IAllocator>();
    sl->UnregisterService<::Devices::ILogicalDevice>();
    sl->UnregisterService<::Devices::IPhysicalDevice>();
    sl->UnregisterService<::Instances::IVulkanInstance>();
  }
};

TEST_F(Dut, CreateVulkanInstance) {
  auto instance = ::Instances::VulkanInstance();
  ASSERT_TRUE(instance.Get() != nullptr);
}

TEST_F(Dut, CreatePhysicalDevice) {
  auto phy_device = ::Devices::PhysicalDevice();
  ASSERT_TRUE(phy_device.Get() != nullptr);
}

TEST_F(Dut, CreateLogicalDevice) {
  auto log_device = ::Devices::LogicalDevice();
  ASSERT_TRUE(log_device.Get() != nullptr);
}

TEST_F(Dut, CreateAllocator) {
  auto allocator = ::Memory::Allocator();
  ASSERT_TRUE(allocator.Get() != nullptr);
}

TEST_F(Dut, CreateCommandPool) {
  auto command_pool = ::CommandPools::CommandPool();
  ASSERT_TRUE(command_pool.Get() != nullptr);
}

TEST_F(Dut, CreateDescriptorPool) {
  auto descriptor_pool = ::DescriptorPools::DescriptorPool();
  ASSERT_TRUE(descriptor_pool.Get() != nullptr);
}

TEST_F(Dut, CreatePipelineCache) {
  auto pipeline_cache = ::Caches::PipelineCache();
  ASSERT_TRUE(pipeline_cache.Get() != nullptr);
}

TEST_F(Dut, CreateIndexBuffer) {
  //const std::vector<uint32_t> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  auto index_buffer = IndexBuffer<uint32_t>(10);
  ASSERT_TRUE(index_buffer.GetVulkanBuffer() != nullptr);
}

TEST_F(Dut, CreateStagingBuffer) {
  const uint8_t uint8_data[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  auto staging_buffer = StagingBuffer(sizeof(uint8_data), uint8_data,
                                      VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
  ASSERT_TRUE(staging_buffer.GetVulkanBuffer() != nullptr);
}

TEST_F(Dut, CreateVertexBuffer) {
  /*
  std::vector<Vulkan::Data::TwoDVertex> vertices = {
    {{-1.0f, -1.0f, 0.0f}, {1.0f, 0.0f}},
    {{1.0f, -1.0f, 0.0f}, {0.0f, 0.0f}},
    {{1.0f, 1.0f, 0.0f}, {0.0f, 1.0f}},
    {{-1.0f, 1.0f, 0.0f}, {1.0f, 1.0f}}};
  */
  auto vertex_buffer =
      VertexBuffer<::Data::TwoDVertex>(4 * sizeof(::Data::TwoDVertex));
  ASSERT_TRUE(vertex_buffer.GetVulkanBuffer() != nullptr);
}

TEST_F(Dut, CreateStorageBuffer) {
  auto storage_buffer = StorageBuffer(100);
  ASSERT_TRUE(storage_buffer.GetVulkanBuffer() != nullptr);
}

TEST_F(Dut, CreateUniformBuffer) {
  auto uniform_buffer = UniformBuffer(100);
  ASSERT_TRUE(uniform_buffer.GetVulkanBuffer() != nullptr);
}

TEST_F(Dut, CreateCommandBuffer) {
  auto command_buf = CommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY);
  ASSERT_TRUE(command_buf.Get() != nullptr);
}

TEST_F(Dut, CreateMaxImageSamplersDescriptorSets) {
  VkDescriptorSetLayoutBinding instanceLayoutBinding = {};
  instanceLayoutBinding.binding = 0;
  instanceLayoutBinding.descriptorType =
      VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
  instanceLayoutBinding.descriptorCount = 1;
  instanceLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
  instanceLayoutBinding.pImmutableSamplers = nullptr;
  const std::vector bindings = {instanceLayoutBinding};
  const std::vector<VkDescriptorBindingFlags> bindingFlags = {};

  auto descriptor_set_layout =
      std::make_shared<DescriptorSetLayout>(bindings, bindingFlags);

  auto descriptor_set = DescriptorSet(descriptor_set_layout);
  ASSERT_TRUE(descriptor_set.Get() != nullptr);
}

TEST_F(Dut, CreateShader) {
  auto shader = Shaders::Shader("2dshader_vert.spv", "2dshader_frag.spv");
  ASSERT_TRUE(shader.GetVertShaderModule() != nullptr);
}

TEST_F(Dut, CreateBlankTexture) {
  auto texture = Textures::Texture(1, 1);
  ASSERT_TRUE(texture.GetImageView() != nullptr);
}

TEST_F(Dut, CreateDepthBufferTexture) {
  auto depth_texture = Textures::DepthBufferTexture(640, 640);
  ASSERT_TRUE(depth_texture.GetImageView() != nullptr);
}

TEST_F(Dut, CreateSwapChainTexture) {
  auto swapchain_texture = Textures::SwapChainTexture(640, 640);
  ASSERT_TRUE(swapchain_texture.GetImageView() != nullptr);
}

TEST_F(Dut, CreateRenderPass) {
  auto render_pass = RenderPasses::RenderPass();
  render_pass.RecreateDepthBuffer(640, 640);
  render_pass.CreateFrameBuffers(640, 640);
  ASSERT_TRUE(render_pass.Get() != nullptr);
}

TEST_F(Dut, CreateSynchronizer) {
  auto sync = Synchronization::Synchronizer(3);
  ASSERT_TRUE(sync.GetImageSemaphores().size() == 3);
}

TEST_F(Dut, CreateTwoDPipeline) {
  auto render_pass = std::make_shared<RenderPasses::RenderPass>();
  auto two_d_pipeline = Pipelines::TwoDPipeline(render_pass);
  ASSERT_TRUE(two_d_pipeline.GetPipeline() != nullptr);
}