#pragma once

// We want to dynamically load Vulkan functions
#define VULKAN_HPP_NO_STRUCT_CONSTRUCTORS
#define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 1
#include <vulkan/vulkan.hpp>
