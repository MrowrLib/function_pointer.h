#pragma once

#include <utility>

#include "FunctionPointerValue.h"
#include "IFunctionPointer.h"

namespace function_pointers {

    template <typename Func>
    class ITypedFunctionPointer;

    template <typename ReturnType, typename... Args>
    struct ITypedFunctionPointer<ReturnType(Args...)> : public IFunctionPointer {
        virtual ReturnType invoke(Args... args) {
            if constexpr (!std::is_same<ReturnType, void>::value) {
                return static_cast<FunctionPointers::FunctionPointerValue<ReturnType>*>(
                           invokeWithArgsArray(
                               new FunctionPointers::IFunctionPointerValue* [sizeof...(Args)] {
                                   new FunctionPointers::FunctionPointerValue<Args>(args)...
                               }
                           )
                )->GetValue();
            } else {
                invokeWithArgsArray(new FunctionPointers::IFunctionPointerValue* [sizeof...(Args)] {
                    new FunctionPointers::FunctionPointerValue<Args>(args)...
                });
            }
        }
    };
}
