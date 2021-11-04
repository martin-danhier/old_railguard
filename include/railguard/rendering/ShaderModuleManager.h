#pragma once

#include "railguard/core/StandaloneManager.h"
#include "railguard/rendering/Ids.h"
#include "railguard/rendering/structs/Storages.h"

// Forward references
#define VK_DEFINE_HANDLE(object) typedef struct object##_T* object;
VK_DEFINE_HANDLE(VkShaderModule)
typedef uint32_t VkFlags;
typedef VkFlags VkShaderStageFlags;

namespace railguard::rendering
{
    class ShaderModuleManager : public core::StandaloneManager<shader_module_id_t, structs::DeviceStorage>
    {
      private:
        // Typedef the parent type to make it easier to call from the methods
        typedef core::StandaloneManager<shader_module_id_t, structs::DeviceStorage> super;

        std::vector<VkShaderStageFlags> _stages;
        std::vector<VkShaderModule> _modules;

        void CleanUp();

      public:
        ShaderModuleManager(structs::DeviceStorage storage, size_t defaultCapacity);
        ~ShaderModuleManager();
        void Clear();

        /**
         * @brief Creates a shader module from a SPIR-V bytecode passed as an argument.
         *
         * @param stage Stage that this shader will be used in.
         * @param codeBuffer SPIR-V bytecode for the shader.
         * @return core::CompleteMatch<shader_module_id_t> match containing the Id of the created module, to be able to retrieve it
         * later.
         */
        [[nodiscard]] core::CompleteMatch<shader_module_id_t> LoadShaderModule(VkShaderStageFlags stage,
                                                                               const std::vector<uint32_t> &codeBuffer);
        /**
         * @brief Creates a shader module from a SPIR-V file of which the path is passed as an argument.
         *
         * @param stage Stage that this shader will be used in.
         * @param filePath Path to a SPIR-V file containing the shader code.
         * @return core::CompleteMatch<shader_module_id_t> match containing the Id of the created module, to be able to retrieve it
         * later.
         */
        [[nodiscard]] core::CompleteMatch<shader_module_id_t> LoadShaderModule(VkShaderStageFlags stage,
                                                                               const std::string &filePath);

        /**
         * @brief Destroys the shader module pointed by the given match
         *
         * @param match match representing the position of the module
         */
        void DestroyShaderModule(const core::Match &match);

        [[nodiscard]] VkShaderStageFlags GetStage(const core::Match &match) const;
        [[nodiscard]] VkShaderModule GetModule(const core::Match &match) const;
    };
} // namespace railguard::rendering