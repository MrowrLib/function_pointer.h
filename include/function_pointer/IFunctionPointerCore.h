#pragma once

#include "IFunctionPointerValue.h"

namespace FunctionPointers {

    struct IFunctionPointerCore {
        virtual ~IFunctionPointerCore() {}
        virtual IFunctionPointerValue* InvokeWithArgsArray(IFunctionPointerValue** args) = 0;
        virtual IFunctionPointerValue* Invoke() { return InvokeWithArgsArray(nullptr); }
    };
}
