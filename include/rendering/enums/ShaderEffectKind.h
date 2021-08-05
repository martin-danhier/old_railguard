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

    inline ShaderEffectKind operator|(ShaderEffectKind a, ShaderEffectKind b)
    {
        return static_cast<ShaderEffectKind>(static_cast<int>(a) | static_cast<int>(b));
    }

    inline ShaderEffectKind operator&(ShaderEffectKind a, ShaderEffectKind b)
    {
        return static_cast<ShaderEffectKind>(static_cast<int>(a) & static_cast<int>(b));
    }
};

// namespace railguard::rendering::enums
