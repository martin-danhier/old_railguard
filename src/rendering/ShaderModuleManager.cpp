#include "../../include/rendering/ShaderModuleManager.h"
#include "../../include/utils/GetError.h"
#include <fstream>
#include <iostream>

namespace railguard::rendering
{
    void ShaderModuleManager::Init(structs::DeviceStorage storage, size_t defaultCapacity)
    {
        // Call parent function
        super::Init(storage, defaultCapacity);

        // Init vectors that weren't initialized by parent
        _stages.reserve(defaultCapacity);
        _modules.reserve(defaultCapacity);
    }

    void ShaderModuleManager::Clear()
    {
        super::Clear();

        // Destroy every module with the device
        for (auto module : _modules)
        {
            _storage.vulkanDevice.destroyShaderModule(module);
        }

        // Once everything is properly destroyed, we can safely clear the vectors and the map
        _stages.clear();
        _modules.clear();
    }

    core::CompleteMatch<shader_module_id_t> ShaderModuleManager::LoadShaderModule(vk::ShaderStageFlagBits stage, const std::vector<uint32_t> &codeBuffer)
    {
        auto match = super::CreateItem();

        vk::ShaderModuleCreateInfo shaderCreateInfo{
            .codeSize = codeBuffer.size() * sizeof(uint32_t),
            .pCode = codeBuffer.data(),
        };
        auto shaderModule = _storage.vulkanDevice.createShaderModule(shaderCreateInfo);

        // Add it to the vectors
        _modules.push_back(shaderModule);
        _stages.push_back(stage);

        return match;
    }

    core::CompleteMatch<shader_module_id_t> ShaderModuleManager::LoadShaderModule(vk::ShaderStageFlagBits stage, const std::string &filePath)
    {
        // Load the SPIR-V code from the given file.

        // Load the binary file with the cursor at the end
        std::ifstream file(filePath, std::ios::ate | std::ios::binary);

        if (!file.is_open())
        {
            // Display error
            std::cerr << "Couldn't load shader \"" << std::string(filePath) << "\": " << utils::GetError() << '\n';
            throw std::runtime_error("Couldn't load shader " + std::string(filePath));
        }

        // Since the cursor is at the end, tellg gives the size of the file
        size_t fileSize = file.tellg();
        // Create a vector long enough to hold the content
        std::vector<uint32_t> buffer(fileSize / sizeof(uint32_t));
        // Cursor at the beginning
        file.seekg(0);
        // Load the file into the buffer
        file.read((char *)buffer.data(), fileSize);
        // Close the file
        file.close();

        return LoadShaderModule(stage, buffer);
    }

//! [Example of derivation of StandaloneManager::DestroyItem]
    void ShaderModuleManager::DestroyShaderModule(const core::Match &match)
    {
        super::DestroyItem(match);

        // Get index
        auto index = match.GetIndex();
        size_t lastIndex = _ids.size() - 1;

        // Destroy the module
        _storage.vulkanDevice.destroyShaderModule(_modules[index]);

        // If the index is smaller then, the destroyed item is not the last and the last one should be moved where
        // the destroyed item was
        if (index < lastIndex)
        {
            _stages[index] = _stages[lastIndex];
            _modules[index] = _modules[lastIndex];
        }

        // Destroy the last item
        _stages.pop_back();
        _modules.pop_back();
    }
//! [Example of derivation of StandaloneManager::DestroyItem]

    vk::ShaderStageFlagBits ShaderModuleManager::GetStage(const core::Match &match) const
    {
        return _stages[match.GetIndex()];
    }

    const vk::ShaderModule ShaderModuleManager::GetModule(const core::Match &match) const
    {
        return _modules[match.GetIndex()];
    }

}