#pragma once

#include "IFunctionPointerValue.h"

namespace function_pointers::FunctionPointers {

    template <typename T>
    class FunctionPointerValue : public IFunctionPointerValue {
        T _value;

    public:
        FunctionPointerValue(const T& value) : _value(value) {}

        void* pointer() const override { return (void*)&_value; }

        T    GetValue() const { return _value; }
        void SetValue(T value) { _value = value; }
    };
}
