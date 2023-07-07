#pragma once

#include "IFunctionPointer.h"

namespace function_pointers {

    template <typename Func>
    struct FunctionPointer;

    template <typename ReturnType, typename... Args>
    struct FunctionPointer<ReturnType(Args...)> : public IFunctionPointer {
        virtual ReturnType invoke(Args... args) = 0;
    };
}