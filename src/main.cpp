#include <GLFW/glfw3.h>
#include "entropy_test.h"
#include <cameras/orthographic_camera.h>
#include <renderers/vulkan_batch_renderer.h>

using namespace Entropy::Graphics::Vulkan::Instances;
using namespace Entropy::Graphics::Vulkan::Devices;
using namespace Entropy::Graphics::Vulkan::Memory;
using namespace Entropy::Graphics::Vulkan::Buffers;
using namespace Entropy::Graphics::Vulkan::Data;
using namespace Entropy::Graphics::Vulkan::CommandPools;
using namespace Entropy::Graphics::Vulkan::CommandBuffers;
using namespace Entropy::Graphics::Vulkan::DescriptorPools;
using namespace Entropy::Graphics::Vulkan::DescriptorSetLayouts;
using namespace Entropy::Graphics::Vulkan::DescriptorSets;
using namespace Entropy::Graphics::Vulkan::Caches;
using namespace Entropy::Graphics::Vulkan::Shaders;
using namespace Entropy::Graphics::Vulkan::Textures;
using namespace Entropy::Graphics::Vulkan::Surfaces;
using namespace Entropy::Graphics::Vulkan::SwapChains;
using namespace Entropy::Graphics::Vulkan::RenderPasses;
using namespace Entropy::Graphics::Vulkan::Pipelines;
using namespace Entropy::Graphics::Vulkan::Synchronization;
using namespace Entropy::Vulkan::Renderers;
using namespace Entropy::ECS;
using namespace Entropy::Assets;
using namespace Entropy::Cameras;

void OnFramebufferResize(GLFWwindow *window, const int width,
                         const int height) {
  if (const auto renderer =
          static_cast<BatchRenderer *>(glfwGetWindowUserPointer(window));
      renderer != nullptr) {
    renderer->Resize(width, height);
    renderer->Render(width, height);
  }
}

flecs::entity CreateSprite(const std::string &path,
                  const glm::vec3 pos = glm::vec3(0.0f),
                  const glm::vec3 dim = glm::vec3(100.0f)) {

  const ServiceLocator *sl = ServiceLocator::GetInstance();
  const auto textureId = sl->getService<IAssetManager>()->LoadAsync<Texture>(path);
   return sl->getService<IWorld>()->Get()->entity()
        .set<Components::Position>({pos})
        .set<Components::Dimension>({dim})
        .set<Components::TwoDQuad>({})
        .set<Components::Texture>({path, textureId, true});
}

void Test() {

  if (!glfwInit()) {
    LOG_INFO(logger_, "Could not initialize GLFW.");
  }

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  GLFWwindow *window =
      glfwCreateWindow(640, 640, "Entropy application", nullptr, nullptr);

  if (!window) {
    LOG_INFO(logger_, "Could not create GLFW window.");
    glfwTerminate();
    return;
  }

    float xscale, yscale;
    glfwGetMonitorContentScale(glfwGetPrimaryMonitor(), &xscale, &yscale);

  const auto surface = std::make_shared<Surface>(window);

  const ServiceLocator *sl = ServiceLocator::GetInstance();
  const auto swapchain = sl->getService<ISwapChain>();

  swapchain->Build(surface, VkExtent2D{static_cast<uint32_t>(640 * xscale), static_cast<uint32_t>(640 * yscale)}, nullptr);

  const auto batch_renderer = new BatchRenderer(static_cast<uint32_t>(640 * xscale), static_cast<uint32_t>(640 * yscale));
  auto bg = CreateSprite("test.png", glm::vec3{500, 500, 0.0f},
               glm::vec3{100, 100, 0.0f});

  glfwSetWindowUserPointer(window, batch_renderer);
  glfwSetFramebufferSizeCallback(window, OnFramebufferResize);

#if 1
  while (!glfwWindowShouldClose(window)) {
    batch_renderer->Render(static_cast<uint32_t>(640 * xscale), static_cast<uint32_t>(640 * yscale));
    glfwPollEvents();
  }
#endif
  delete batch_renderer;
  // Cleanup
  glfwTerminate();
}

int main() {
  // Initialize quill, logging library
  InitializeQuill();

  // Register vulkan services
  ServiceLocator *sl = ServiceLocator::GetInstance();
  sl->RegisterService(std::make_shared<VulkanInstance>());
  sl->RegisterService(std::make_shared<PhysicalDevice>());
  sl->RegisterService(std::make_shared<LogicalDevice>());
  sl->RegisterService(std::make_shared<Allocator>());
  sl->RegisterService(std::make_shared<CommandPool>());
  sl->RegisterService(std::make_shared<DescriptorPool>());
  sl->RegisterService(std::make_shared<PipelineCache>());
  sl->RegisterService(std::make_shared<SwapChain>());
  sl->RegisterService(std::make_shared<World>());
  sl->RegisterService(std::make_shared<AssetManager>());
  sl->RegisterService(std::make_shared<CameraManager>());

  /*

  const auto vk_instance = sl->getService<IVulkanInstance>();
  const auto physical_device = sl->getService<IPhysicalDevice>();
  const auto logical_device = sl->getService<ILogicalDevice>();
  const auto allocator = sl->getService<IAllocator>();
  const auto command_pool = sl->getService<ICommandPool>();
  const auto descriptor_pool = sl->getService<IDescriptorPool>();
  const auto pipeline_cache = sl->getService<IPipelineCache>();

  const std::vector<uint32_t> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  auto index_buffer = IndexBuffer(data);

  const uint8_t uint8_data[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  auto staging_buffer = StagingBuffer(sizeof(uint8_data), uint8_data,
                                      VK_BUFFER_USAGE_TRANSFER_SRC_BIT);

  const std::vector<ThreeDAnimVertex> vertices = {{{-1.0f, -1.0f, 0.0f},
                                                   {1.0f, 1.0f, 1.0f},
                                                   {1.0f, 0.0f, 0.0f, 1.0f},
                                                   {0.0f, 0.0f},
                                                   {1.0f, 0.0f, 0.0f, 1.0f},
                                                   {1.0f, 0.0f, 0.0f, 1.0f}},
                                                  {{-1.0f, -1.0f, 0.0f},
                                                   {1.0f, 1.0f, 1.0f},
                                                   {1.0f, 0.0f, 0.0f, 1.0f},
                                                   {0.0f, 0.0f},
                                                   {1.0f, 0.0f, 0.0f, 1.0f},
                                                   {1.0f, 0.0f, 0.0f, 1.0f}},
                                                  {{-1.0f, -1.0f, 0.0f},
                                                   {1.0f, 1.0f, 1.0f},
                                                   {1.0f, 0.0f, 0.0f, 1.0f},
                                                   {0.0f, 0.0f},
                                                   {1.0f, 0.0f, 0.0f, 1.0f},
                                                   {1.0f, 0.0f, 0.0f, 1.0f}},
                                                  {{-1.0f, -1.0f, 0.0f},
                                                   {1.0f, 1.0f, 1.0f},
                                                   {1.0f, 0.0f, 0.0f, 1.0f},
                                                   {0.0f, 0.0f},
                                                   {1.0f, 0.0f, 0.0f, 1.0f},
                                                   {1.0f, 0.0f, 0.0f, 1.0f}}};
  auto vertex_buffer = VertexBuffer(vertices);

  auto storage_buffer = StorageBuffer(100);
  auto uniform_buffer = UniformBuffer(100);

  auto command_buf = CommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY);

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

  std::vector<std::shared_ptr<DescriptorSet>> descriptor_sets;
  for (uint32_t i = 0; i < MAX_DESCRIPTOR_SETS - (10); i++) {
    descriptor_sets.push_back(
        std::make_shared<DescriptorSet>(descriptor_set_layout));
  }

  auto shader = Shader("2d_shader_vert.spv", "2d_shader_frag.spv");

  auto texture = Texture("test.png");
  auto depth_texture = DepthBufferTexture(640, 640);
  auto swapchain_texture = SwapChainTexture(640, 640);

  const auto texture_manager = sl->getService<ITextureManager>();
  auto tex_e = texture_manager->LoadTexture("test.png");
  texture_manager->ReleaseTexture(tex_e);

  auto render_pass = std::make_shared<RenderPass>();
  render_pass->RecreateDepthBuffer(640, 640);
  render_pass->CreateFrameBuffers(640, 640);

  auto two_d_pipeline = TwoDPipeline(render_pass);

  auto sync = Synchronizer(3);

   */

  const auto camera_manager = sl->getService<ICameraManager>();
  camera_manager->SetCurrentCamera(std::make_shared<OrthographicCamera>());

  Test();

  // Unregister services
  sl->UnregisterService<ICameraManager>();
  sl->UnregisterService<IAssetManager>();
  sl->UnregisterService<IWorld>();
  sl->UnregisterService<ISwapChain>();
  sl->UnregisterService<IPipelineCache>();
  sl->UnregisterService<IDescriptorPool>();
  sl->UnregisterService<ICommandPool>();
  sl->UnregisterService<IAllocator>();
  sl->UnregisterService<ILogicalDevice>();
  sl->UnregisterService<IPhysicalDevice>();
  sl->UnregisterService<IVulkanInstance>();

  return EXIT_SUCCESS;
}