#pragma once

#include <cinttypes>

namespace railguard::rendering
{

    // Use a typedef to specify which type will be used for ids
    // That way, if we need to change that type, we only need to do it here

    typedef uint32_t material_id_t;
    typedef uint32_t material_template_id_t;
    typedef uint32_t mesh_id_t;
    typedef uint32_t model_id_t;
    typedef uint32_t shader_effect_id_t;
    typedef uint32_t shader_module_id_t;
    typedef uint32_t swapchain_id_t;

} // namespace railguard::rendering
