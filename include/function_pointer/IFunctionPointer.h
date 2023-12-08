#pragma once

#include <utility>

#include "FunctionPointerValue.h"
#include "IFunctionPointerBase.h"

namespace FunctionPointers {

    template <typename Func>
    class IFunctionPointer;

    template <typename ReturnType, typename... Args>
    struct IFunctionPointer<ReturnType(Args...)> : public IFunctionPointerBase {
        virtual ReturnType invoke(Args... args) const {
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
