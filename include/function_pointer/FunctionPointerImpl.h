#pragma once

#include <memory>

#include "FunctionPointer.h"
#include "FunctionalFunctionPointer.h"
#include "MemberFunctionPointer.h"
#include "StaticFunctionPointer.h"

namespace function_pointers::FunctionPointers {

    template <typename ReturnType, typename... Args>
    class FunctionPointerImpl : public FunctionPointer<ReturnType(Args...)> {
        std::unique_ptr<IFunctionPointer> _functionPointer;

    public:
        FunctionPointerImpl(ReturnType (*func)(Args...)) {
            _functionPointer =
                std::make_unique<FunctionPointers::StaticFunctionPointer<ReturnType, Args...>>(func
                );
        }

        FunctionPointerImpl(std::function<ReturnType(Args...)> func) {
            _functionPointer =
                std::make_unique<FunctionPointers::FunctionalFunctionPointer<ReturnType, Args...>>(
                    func
                );
        }

        template <typename T>
        FunctionPointerImpl(T* instance, ReturnType (T::*func)(Args...)) {
            _functionPointer =
                std::make_unique<FunctionPointers::MemberFunctionPointer<T, ReturnType, Args...>>(
                    instance, func
                );
        }

        FunctionPointers::IFunctionPointerValue* invokeWithArgsArray(
            FunctionPointers::IFunctionPointerValue** args
        ) override {
            return nullptr;
        }

        virtual ReturnType invoke(Args... args) {
            if constexpr (!std::is_same<ReturnType, void>::value) {
                return static_cast<FunctionPointers::FunctionPointerValue<ReturnType>*>(
                           _functionPointer->invokeWithArgsArray(
                               new FunctionPointers::IFunctionPointerValue* [sizeof...(Args)] {
                                   new FunctionPointers::FunctionPointerValue<Args>(args)...
                               }
                           )
                )->GetValue();
            } else {
                _functionPointer->invokeWithArgsArray(
                    new FunctionPointers::IFunctionPointerValue* [sizeof...(Args)] {
                        new FunctionPointers::FunctionPointerValue<Args>(args)...
                    }
                );
            }
        }
    };
}