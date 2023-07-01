#pragma once

#include "IFunctionPointerValue.h"

namespace FunctionPointers {

    struct IFunctionPointer {
        virtual ~IFunctionPointer() {}
        virtual IFunctionPointerValue* Invoke(IFunctionPointerValue** args) = 0;
        virtual IFunctionPointerValue* Invoke() { return Invoke(nullptr); }
    };
}
