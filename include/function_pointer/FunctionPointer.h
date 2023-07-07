#pragma once

#include "IFunctionPointer.h"

namespace function_pointers {

    template <typename ReturnType, typename... Args>
    struct FunctionPointer : public IFunctionPointer {
        virtual ReturnType invoke(Args... args) = 0;
    };
}