#pragma once

#include <memory>

#include "IFunctionPointerValueArrayDeleter.h"
#include "MemberFunctionPointer.h"
#include "StaticFunctionPointer.h"

namespace FunctionPointers {

    struct FunctionPointer {
        template <typename ReturnType, typename... Args>
        static IFunctionPointer* make_new(ReturnType (*func)(Args...)) {
            return new StaticFunctionPointer<ReturnType, Args...>(func);
        }

        template <typename ReturnType, typename... Args>
        static std::unique_ptr<IFunctionPointer> make_unique(ReturnType (*func)(Args...)) {
            return std::make_unique<StaticFunctionPointer<ReturnType, Args...>>(func);
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
    };
}
