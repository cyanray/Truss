#pragma once

namespace Truss
{
    template<typename TResult, typename...TArgs>
    struct IsFunctionPointer {};

    template<typename TResult, typename...TArgs>
    struct IsFunctionPointer<TResult(&)(TArgs...)>
    {
        const static bool value{ true };
    };

    template<typename TResult, typename...TArgs>
    struct IsFunctionPointer<TResult(*)(TArgs...)>
    {
        const static bool value{ true };
    };

    template<typename TFunc>
    concept FunctionPointer = IsFunctionPointer<TFunc>::value;
}