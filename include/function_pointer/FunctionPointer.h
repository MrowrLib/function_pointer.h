#pragma once

#include <memory>

#include "FunctionalFunctionPointer.h"
#include "MemberFunctionPointer.h"
#include "StaticFunctionPointer.h"

namespace FunctionPointers {

    template <typename Func>
    struct FunctionPointer;

    template <typename ReturnType, typename... Args>
    struct FunctionPointer<ReturnType(Args...)> : public IFunctionPointer<ReturnType(Args...)> {
        std::unique_ptr<IFunctionPointerBase> _functionPointer;

    public:
        FunctionPointer(ReturnType (*func)(Args...)) {
            _functionPointer =
                std::make_unique<FunctionPointers::StaticFunctionPointer<ReturnType(Args...)>>(func
                );
        }

        FunctionPointer(std::function<ReturnType(Args...)> func) {
            _functionPointer =
                std::make_unique<FunctionPointers::FunctionalFunctionPointer<ReturnType(Args...)>>(
                    func
                );
        }

        template <typename T>
        FunctionPointer(T* instance, ReturnType (T::*func)(Args...)) {
            _functionPointer =
                std::make_unique<FunctionPointers::MemberFunctionPointer<T, ReturnType, Args...>>(
                    instance, func
                );
        }

        FunctionPointers::IFunctionPointerValue* invokeWithArgsArray(
            FunctionPointers::IFunctionPointerValue** args
        ) override {
            return _functionPointer->invokeWithArgsArray(args);
        }

        virtual IFunctionPointerBase* inner_function_pointer() { return _functionPointer.get(); }
    };
}