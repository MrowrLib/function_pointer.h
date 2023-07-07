#pragma once

#include <memory>

#include "FunctionalFunctionPointer.h"
#include "IFunctionPointer.h"
#include "MemberFunctionPointer.h"
#include "StaticFunctionPointer.h"

namespace function_pointers {

    template <typename Func>
    struct FunctionPointer;

    template <typename ReturnType, typename... Args>
    struct FunctionPointer<ReturnType(Args...)> : public IFunctionPointer {
        std::unique_ptr<IFunctionPointer> _functionPointer;

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
                std::make_unique<FunctionPointers::MemberFunctionPointer<T, ReturnType(Args...)>>(
                    instance, func
                );
        }

        FunctionPointers::IFunctionPointerValue* invokeWithArgsArray(
            FunctionPointers::IFunctionPointerValue** args
        ) override {
            return _functionPointer->invokeWithArgsArray(args);
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

        virtual IFunctionPointer* inner_function_pointer() { return _functionPointer.get(); }
    };
}