#pragma once

#include "IFunctionPointerValue.h"

namespace FunctionPointers {

    struct IFunctionPointer {
        virtual ~IFunctionPointer() {}
        virtual IFunctionPointerValue* InvokeWithArgsArray(IFunctionPointerValue** args) = 0;
        virtual IFunctionPointerValue* Invoke() { return InvokeWithArgsArray(nullptr); }
    };
}
