#pragma once

#include <vulkan/vulkan_raii.hpp>

namespace dust::render::vulkan
{

struct SuitableQueueFamily
{
	uint32_t queueFamily, queueCount;
	vk::QueueFlags queueFlags;
	bool presentSupport;
};

struct SuitablePhysicalDevice
{
	vk::raii::PhysicalDevice physicalDevice;
	uint32_t deviceIndex;
	std::vector<SuitableQueueFamily> queueFamilies;
};

} // namespace dust::render::vulkan
