#pragma once

#include "../includes/Vulkan.h"
#include "../core/StandaloneManager.h"
#include "./structs/Storages.h"

namespace railguard::rendering
{
    // Use a typedef to specify which type will be used for shader module ids
    // That way, if we need to change that type, we only need to do it here
    typedef uint32_t shader_module_id_t;

    class ShaderModuleManager : public core::StandaloneManager<shader_module_id_t, structs::DeviceStorage>
    {
    private:
        // Typedef the parent type to make it easier to call from the methods
        typedef core::StandaloneManager<shader_module_id_t, structs::DeviceStorage> super;

        std::vector<vk::ShaderStageFlagBits> _stages;
        std::vector<vk::ShaderModule> _modules;

    public:
        void Init(structs::DeviceStorage storage, size_t defaultCapacity = 5);
        void Clear();

        /**
         * @brief Creates a shader module from a SPIR-V bytecode passed as an argument.
         *
         * @param stage Stage that this shader will be used in.
         * @param codeBuffer SPIR-V bytecode for the shader.
         * @return core::CompleteMatch<shader_module_id_t> match containing the Id of the created module, to be able to retreive it later.
         */
        core::CompleteMatch<shader_module_id_t> LoadShaderModule(vk::ShaderStageFlagBits stage, const std::vector<uint32_t> &codeBuffer);
        /**
         * @brief Creates a shader module from a SPIR-V file of which the path is passed as an argument.
         *
         * @param stage Stage that this shader will be used in.
         * @param filePath Path to a SPIR-V file containing the shader code.
         * @return core::CompleteMatch<shader_module_id_t> match containing the Id of the created module, to be able to retreive it later.
         */
        core::CompleteMatch<shader_module_id_t> LoadShaderModule(vk::ShaderStageFlagBits stage, const std::string &filePath);

        /**
         * @brief Destroys the shader module pointed by the given match
         *
         * @param match match representing the position of the module
         */
        void DestroyShaderModule(const core::Match &match);

        [[nodiscard]] vk::ShaderStageFlagBits GetStage(const core::Match &match) const;
        [[nodiscard]] vk::ShaderModule GetModule(const core::Match &match) const;

        
    };
}