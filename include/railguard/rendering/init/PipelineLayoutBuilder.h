#pragma once

#define VK_DEFINE_HANDLE(object) typedef struct object##_T* object;
VK_DEFINE_HANDLE(VkPipelineLayout)
VK_DEFINE_HANDLE(VkDevice)

namespace railguard::rendering::init
{
    /**
     * @brief Helper class that can be used to generate vk::PipelineLayout objects easily.
     */
    class PipelineLayoutBuilder
    {
      private:
      public:
        [[nodiscard]] static VkPipelineLayout Build(const VkDevice &vulkanDevice);
    };
} // namespace railguard::rendering::init
