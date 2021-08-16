#pragma once

#include <railguard/core/StandaloneManager.h>
#include <railguard/includes/Vulkan.h>
#include <railguard/rendering/Ids.h>
#include <railguard/rendering/enums/ShaderEffectKind.h>

namespace railguard
{
    namespace core
    {
        class WindowManager;
    }
    namespace rendering
    {
        namespace enums
        {
            enum class ShaderEffectKind;
        }
        namespace init
        {
            struct ShaderEffectInitInfo;
        }

        /**
         * @brief Storage that is used to store the device as well as a pointer to the shader module manager.
         */
        struct ShaderEffectManagerStorage
        {
            vk::Device vulkanDevice                              = nullptr;
            vk::RenderPass renderPass                            = nullptr;
            const class ShaderModuleManager *shaderModuleManager = nullptr;
            const core::WindowManager *windowManager             = nullptr;
        };

        /**
         * @brief A "shader effect" is a configuration of pipeline layouts, shaders and descriptor set.
         * Materials can use several shader effects (e.g. for forward and shadow rendering), and a single shader effect can be used
         * by several pipelines (for example a "default lit" shader effect).
         */
        class ShaderEffectManager : public core::StandaloneManager<shader_effect_id_t, ShaderEffectManagerStorage>
        {
          private:
            // Typedef the parent type to make it easier to call from the methods
            typedef core::StandaloneManager<shader_effect_id_t, ShaderEffectManagerStorage> super;

            std::vector<vk::PipelineLayout> _pipelineLayouts;
            std::vector<std::vector<shader_module_id_t>> _shaderStages;
            std::vector<vk::Pipeline> _pipelines;
            std::vector<enums::ShaderEffectKind> _effectKinds;

            void CleanUp();

          public:
            ShaderEffectManager(ShaderEffectManagerStorage storage, size_t defaultCapacity);
            ~ShaderEffectManager();
            /**
             * @brief Destroys every remaining shader effect.
             */
            void Clear() override;

            /**
             * @brief Create a Shader Effect and add it to the vectors. The effect will not be built yet, meaning the pipeline will be
             * null. Call BuildEffect() to build it, or set the "build" parameter to true.
             *
             * @param buildEffectAfterCreation if true, the Pipeline will be built directly after creation.
             *      Otherwise, it will be left null and BuildEffect() will need to be called later.
             *
             * @return CompleteMatch A match allowing either to retrieve the ID of the effect or to use it with another function
             */
            [[nodiscard]] core::CompleteMatch<shader_effect_id_t> CreateShaderEffect(const init::ShaderEffectInitInfo &initInfo,
                                                                                     bool buildEffectAfterCreation = false);

            vk::Pipeline BuildEffect(const core::Match &match);

            /**
             * @brief Destroys the shader effect pointed by the given match
             *
             * @param match match representing the position of the effect
             */
            void DestroyShaderEffect(const core::Match &match);

            void Bind(const core::Match &match, const vk::CommandBuffer &cmd) const;
            [[nodiscard]] std::vector<shader_effect_id_t> GetEffectsOfKind(enums::ShaderEffectKind kind) const;

            [[nodiscard]] vk::PipelineLayout GetPipelineLayout(const core::Match &match) const;
            [[nodiscard]] vk::Pipeline GetPipeline(const core::Match &match) const;
            [[nodiscard]] std::vector<shader_module_id_t> GetShaderStages(const core::Match &match) const;
            [[nodiscard]] enums::ShaderEffectKind GetEffectKind(const core::Match &match) const;
        };
    } // namespace rendering
} // namespace railguard