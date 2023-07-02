#pragma once

#include <memory>

#include "FunctionPointer.h"
#include "IFunctionPointerValueArrayDeleter.h"
#include "MemberFunctionPointer.h"
#include "StaticFunctionPointer.h"


namespace FunctionPointers {

    struct function_pointer {
        template <typename ReturnType, typename... Args>
        static IFunctionPointer* make_new(ReturnType (*func)(Args...)) {
            return new StaticFunctionPointer<ReturnType, Args...>(func);
        }

        template <typename T, typename ReturnType, typename... Args>
        static IFunctionPointer* make_new(T* object, ReturnType (T::*func)(Args...)) {
            return new MemberFunctionPointer<T, ReturnType, Args...>(object, func);
        }

        template <typename ReturnType, typename... Args>
        static FunctionPointer make_unique(ReturnType (*func)(Args...)) {
            return std::make_unique<StaticFunctionPointer<ReturnType, Args...>>(func);
        }

        template <typename T, typename ReturnType, typename... Args>
        static FunctionPointer make_unique(T* object, ReturnType (T::*func)(Args...)) {
            return std::make_unique<MemberFunctionPointer<T, ReturnType, Args...>>(object, func);
        }

        template <typename... Args>
        static IFunctionPointerValue** make_new_args(Args&&... args) {
            constexpr auto argsCount = sizeof...(Args);
            return new IFunctionPointerValue* [argsCount + 1] {
                new FunctionPointerValue<std::decay_t<Args>>(std::forward<Args>(args))...
            };
        }

        template <typename... Args>
        static std::unique_ptr<IFunctionPointerValue* [], IFunctionPointerValueArrayDeleter>
        make_unique_args(Args&&... args) {
            return std::unique_ptr<IFunctionPointerValue*[], IFunctionPointerValueArrayDeleter>(
                make_new_args(std::forward<Args>(args)...)
            );
        }

        template <typename ReturnType, typename... Args>
        static std::unique_ptr<IFunctionPointerValue> invoke(
            ReturnType (*func)(Args...), Args&&... args
        ) {
            return std::unique_ptr<IFunctionPointerValue>(
                make_new(func)->Invoke(make_unique_args(std::forward<Args>(args)...).get())
            );
        }

        template <typename T, typename ReturnType, typename... Args>
        static std::unique_ptr<IFunctionPointerValue> invoke(
            T* object, ReturnType (T::*func)(Args...), Args&&... args
        ) {
            return std::unique_ptr<IFunctionPointerValue>(
                make_new(object, func)->Invoke(make_unique_args(std::forward<Args>(args)...).get())
            );
        }
    };
}
