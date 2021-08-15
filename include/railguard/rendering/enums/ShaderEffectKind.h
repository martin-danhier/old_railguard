#pragma once

namespace railguard::rendering::enums
{
    enum class ShaderEffectKind
    {
        None = 0,
        Forward = 1,
        Shadow = 2,
        Transparent = 4,
        PBR = 8
    };

    ShaderEffectKind operator|(ShaderEffectKind a, ShaderEffectKind b);

    ShaderEffectKind operator&(ShaderEffectKind a, ShaderEffectKind b);
}

// namespace railguard::rendering::enums
