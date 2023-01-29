#include "Truss/Utils/SimpleReflection.hpp"

using namespace Truss;

bool SimpleReflection::IsEmpty() const noexcept
{
    return m_FuncionMap.empty();
}

size_t SimpleReflection::Size() const noexcept
{
    return m_FuncionMap.size();
}
