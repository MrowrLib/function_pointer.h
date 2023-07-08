#pragma once

#include "IFunctionPointer.h"

namespace FunctionPointers {

    template <typename ClassT, typename ReturnType, typename... Args>
    struct IMemberFunctionPointer : public IFunctionPointer<ReturnType(Args...)> {};
}
