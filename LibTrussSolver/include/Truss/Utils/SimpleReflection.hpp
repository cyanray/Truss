#pragma once
#include <string>
#include <string_view>
#include <unordered_map>
#include <functional>
#include <memory>
#include <any>
#include <concepts>
#include "FunctionPointer.hpp"
using std::string;
using std::string_view;

namespace Truss
{
    class SimpleReflection
    {
    public:
        template<typename TReturn, typename...TArgs>
        TReturn Invoke(const string& name, TArgs&&... args)
        {
            using TFuncPtr = TReturn (*)(TArgs...);
            auto& func_any =  m_FuncionMap.at(name);
            TFuncPtr func = std::any_cast<TFuncPtr>(func_any);
            return func(std::forward<TArgs>(args)...);
        }

        template<FunctionPointer TFunc>
        void Register(const string& name, TFunc&& function)
        {
            m_FuncionMap.insert({name, function});
        }

        [[nodiscard]] bool IsEmpty() const noexcept;

        [[nodiscard]] size_t Size() const noexcept;

    private:
        std::unordered_map<string, std::any> m_FuncionMap;
    };
}