#pragma once

#include "IFunctionPointerValue.h"

namespace FunctionPointers {

    struct IFunctionPointerBase {
        virtual ~IFunctionPointerBase() = default;
        virtual FunctionPointers::IFunctionPointerValue* invokeWithArgsArray(
            FunctionPointers::IFunctionPointerValue** args
        ) = 0;
    };
}
