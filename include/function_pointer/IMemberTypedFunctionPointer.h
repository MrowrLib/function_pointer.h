#pragma once

#include "ITypedFunctionPointer.h"

namespace function_pointers::FunctionPointers {

    template <typename ClassT, typename ReturnType, typename... Args>
    struct IMemberTypedFunctionPointer : public ITypedFunctionPointer<ReturnType(Args...)> {};
}
