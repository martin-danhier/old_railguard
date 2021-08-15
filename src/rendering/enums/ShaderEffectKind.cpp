#include <railguard/rendering/enums/ShaderEffectKind.h>

namespace railguard::rendering::enums
{
    ShaderEffectKind operator|(ShaderEffectKind a, ShaderEffectKind b)
    {
        return static_cast<ShaderEffectKind>(static_cast<int>(a) | static_cast<int>(b));
    }

    ShaderEffectKind operator&(ShaderEffectKind a, ShaderEffectKind b)
    {
        return static_cast<ShaderEffectKind>(static_cast<int>(a) & static_cast<int>(b));
    }

} // namespace railguard::rendering::enums