#pragma once

#include <concepts>

// Redefine integral concepts because it is not yet available on some compilers
namespace utils
{
    template<typename _Tp>
    concept integral = std::is_integral_v<_Tp>;
}