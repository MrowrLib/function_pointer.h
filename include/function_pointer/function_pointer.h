#pragma once

#include <memory>
#include <utility>

#include "FunctionPointer.h"
#include "FunctionalFunctionPointer.h"
#include "IFunctionPointerValueArrayDeleter.h"
#include "MemberFunctionPointer.h"
#include "StaticFunctionPointer.h"
#include "function_traits.h"

namespace FunctionPointers {

    class function_pointer {
        template <typename F, typename ReturnType, typename ArgsTuple, std::size_t... I>
        static IFunctionPointer* make_new_function_impl(F&& f, std::index_sequence<I...>) {
            return new FunctionalFunctionPointer<ReturnType, std::tuple_element_t<I, ArgsTuple>...>(
                std::forward<F>(f)
            );
        }

        template <typename F, typename ReturnType, typename ArgsTuple, std::size_t... I>
        static FunctionPointer make_unique_function_impl(F&& f, std::index_sequence<I...>) {
            return std::make_unique<
                FunctionalFunctionPointer<ReturnType, std::tuple_element_t<I, ArgsTuple>...>>(
                std::forward<F>(f)
            );
        }

    public:
        template <typename ReturnType, typename... Args>
        static IFunctionPointer* make_new(ReturnType (*func)(Args...)) {
            return new StaticFunctionPointer<ReturnType, Args...>(func);
        }

        template <typename T, typename ReturnType, typename... Args>
        static IFunctionPointer* make_new(T* object, ReturnType (T::*func)(Args...)) {
            return new MemberFunctionPointer<T, ReturnType, Args...>(object, func);
        }

        template <typename F>
        static IFunctionPointer* make_new(F&& f) {
            using Traits     = function_traits<F>;
            using ReturnType = typename Traits::return_type;
            using ArgsTuple  = typename Traits::args_tuple;
            return make_new_function_impl<F, ReturnType, ArgsTuple>(
                std::forward<F>(f), std::make_index_sequence<std::tuple_size_v<ArgsTuple>>{}
            );
        }

        template <typename ReturnType, typename... Args>
        static FunctionPointer make_unique(ReturnType (*func)(Args...)) {
            return std::make_unique<StaticFunctionPointer<ReturnType, Args...>>(func);
        }

        template <typename T, typename ReturnType, typename... Args>
        static FunctionPointer make_unique(T* object, ReturnType (T::*func)(Args...)) {
            return std::make_unique<MemberFunctionPointer<T, ReturnType, Args...>>(object, func);
        }

        template <typename F>
        static FunctionPointer make_unique(F&& f) {
            using Traits     = function_traits<F>;
            using ReturnType = typename Traits::return_type;
            using ArgsTuple  = typename Traits::args_tuple;
            return make_unique_function_impl<F, ReturnType, ArgsTuple>(
                std::forward<F>(f), std::make_index_sequence<std::tuple_size_v<ArgsTuple>>{}
            );
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
            return std::unique_ptr<IFunctionPointerValue>(make_new(func)->InvokeWithArgsArray(
                make_unique_args(std::forward<Args>(args)...).get()
            ));
        }

        template <typename T, typename ReturnType, typename... Args>
        static std::unique_ptr<IFunctionPointerValue> invoke(
            T* object, ReturnType (T::*func)(Args...), Args&&... args
        ) {
            return std::unique_ptr<IFunctionPointerValue>(
                make_new(object, func)
                    ->InvokeWithArgsArray(make_unique_args(std::forward<Args>(args)...).get())
            );
        }

        template <typename... Args>
        static std::unique_ptr<IFunctionPointerValue> invoke(
            IFunctionPointer* functionPointer, Args&&... args
        ) {
            return std::unique_ptr<IFunctionPointerValue>(functionPointer->InvokeWithArgsArray(
                make_unique_args(std::forward<Args>(args)...).get()
            ));
        }

        template <typename... Args>
        static std::unique_ptr<IFunctionPointerValue> invoke(
            FunctionPointer& functionPointer, Args&&... args
        ) {
            return std::unique_ptr<IFunctionPointerValue>(functionPointer->InvokeWithArgsArray(
                make_unique_args(std::forward<Args>(args)...).get()
            ));
        }
    };
}
