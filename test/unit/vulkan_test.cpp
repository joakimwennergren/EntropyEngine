#include <gtest/gtest.h>

#include "vulkan/descriptorpools/idescriptorpool.h"
#include "vulkan/descriptorpools/descriptorpool.h"

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

using namespace Entropy::Graphics::Vulkan;

class Dut : public ::testing::Test {
protected:
    void SetUp() override {
        InitializeQuill();
        ServiceLocator *sl = ServiceLocator::GetInstance();
        sl->RegisterService(std::make_shared<::Instances::VulkanInstance>());
        sl->RegisterService(std::make_shared<::Devices::PhysicalDevice>());
        sl->RegisterService(std::make_shared<::Devices::LogicalDevice>());
        sl->RegisterService(std::make_shared<::Memory::Allocator>());
        sl->RegisterService(std::make_shared<::CommandPools::CommandPool>());
        sl->RegisterService(std::make_shared<::DescriptorPools::DescriptorPool>());
    }

    void TearDown() override {
        ServiceLocator *sl = ServiceLocator::GetInstance();
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
