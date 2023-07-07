#pragma once

#include "IFunctionPointerValue.h"

namespace function_pointers {

    struct IFunctionPointer {
        virtual ~IFunctionPointer() {}
        virtual FunctionPointers::IFunctionPointerValue* invokeWithArgsArray(
            FunctionPointers::IFunctionPointerValue** args
        ) = 0;
    };
}
