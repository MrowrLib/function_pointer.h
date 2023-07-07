#pragma once

#include <memory>
#include <utility>

#include "FunctionPointer.h"
#include "FunctionalFunctionPointer.h"
#include "IFunctionPointerValueArrayDeleter.h"
#include "MemberFunctionPointer.h"
#include "StaticFunctionPointer.h"
#include "function_traits.h"

namespace function_pointers {

    class function_pointer {
        template <typename F, typename ReturnType, typename ArgsTuple, std::size_t... I>
        static IFunctionPointer* make_new_untyped_function_impl(F&& f, std::index_sequence<I...>) {
            return new FunctionPointers::FunctionalFunctionPointer<
                ReturnType, std::tuple_element_t<I, ArgsTuple>...>(std::forward<F>(f));
        }

        template <typename F, typename ReturnType, typename ArgsTuple, std::size_t... I>
        static std::unique_ptr<IFunctionPointer>
        make_unique_untyped_function_impl(F&& f, std::index_sequence<I...>) {
            return std::make_unique<FunctionPointers::FunctionalFunctionPointer<
                ReturnType, std::tuple_element_t<I, ArgsTuple>...>>(std::forward<F>(f));
        }

        template <typename F, typename ReturnType, typename ArgsTuple, std::size_t... I>
        static FunctionPointer<ReturnType, std::tuple_element_t<I, ArgsTuple>...>*
        make_new_function_impl(F&& f, std::index_sequence<I...>) {
            return new FunctionPointer<ReturnType, std::tuple_element_t<I, ArgsTuple>...>(
                std::function<ReturnType(std::tuple_element_t<I, ArgsTuple>...)>(std::forward<F>(f))
            );
        }

        template <typename F, typename ReturnType, typename ArgsTuple, std::size_t... I>
        static std::unique_ptr<FunctionPointer<ReturnType, std::tuple_element_t<I, ArgsTuple>...>>
        make_unique_function_impl(F&& f, std::index_sequence<I...>) {
            return std::make_unique<
                FunctionPointer<ReturnType, std::tuple_element_t<I, ArgsTuple>...>>(
                std::function<ReturnType(std::tuple_element_t<I, ArgsTuple>...)>(std::forward<F>(f))
            );
        }

    public:
        template <typename ReturnType, typename... Args>
        static FunctionPointer<ReturnType, Args...>* make_new(ReturnType (*func)(Args...)) {
            return new FunctionPointer<ReturnType, Args...>(func);
        }

        template <typename T, typename ReturnType, typename... Args>
        static FunctionPointer<ReturnType, Args...>* make_new(
            T* object, ReturnType (T::*func)(Args...)
        ) {
            return new FunctionPointer<ReturnType, Args...>(object, func);
        }

        template <typename ReturnType, typename... Args>
        static FunctionPointer<ReturnType, Args...>* make_new(
            std::function<ReturnType(Args...)> func
        ) {
            return new FunctionPointer<ReturnType, Args...>(func);
        }

        template <typename F>
        auto static make_new(F&& f) {
            using Traits     = FunctionPointers::function_traits<F>;
            using ReturnType = typename Traits::return_type;
            using ArgsTuple  = typename Traits::args_tuple;
            return make_new_function_impl<F, ReturnType, ArgsTuple>(
                std::forward<F>(f), std::make_index_sequence<std::tuple_size_v<ArgsTuple>>{}
            );
        }

        template <typename ReturnType, typename... Args>
        static std::unique_ptr<FunctionPointer<ReturnType, Args...>> make_unique(
            ReturnType (*func)(Args...)
        ) {
            return std::make_unique<FunctionPointer<ReturnType, Args...>>(func);
        }

        template <typename T, typename ReturnType, typename... Args>
        static std::unique_ptr<FunctionPointer<ReturnType, Args...>> make_unique(
            T* object, ReturnType (T::*func)(Args...)
        ) {
            return std::make_unique<FunctionPointer<ReturnType, Args...>>(object, func);
        }

        template <typename ReturnType, typename... Args>
        static std::unique_ptr<FunctionPointer<ReturnType, Args...>> make_unique(
            std::function<ReturnType(Args...)> func
        ) {
            return std::make_unique<FunctionPointer<ReturnType, Args...>>(func);
        }

        template <typename F>
        auto static make_unique(F&& f) {
            using Traits     = FunctionPointers::function_traits<F>;
            using ReturnType = typename Traits::return_type;
            using ArgsTuple  = typename Traits::args_tuple;
            return make_unique_function_impl<F, ReturnType, ArgsTuple>(
                std::forward<F>(f), std::make_index_sequence<std::tuple_size_v<ArgsTuple>>{}
            );
        }

        template <typename ReturnType, typename... Args>
        static IFunctionPointer* make_new_untyped(ReturnType (*func)(Args...)) {
            return new FunctionPointers::StaticFunctionPointer<ReturnType, Args...>(func);
        }

        template <typename T, typename ReturnType, typename... Args>
        static IFunctionPointer* make_new_untyped(T* object, ReturnType (T::*func)(Args...)) {
            return new FunctionPointers::MemberFunctionPointer<T, ReturnType, Args...>(
                object, func
            );
        }

        template <typename F>
        static IFunctionPointer* make_new_untyped(F&& f) {
            using Traits     = FunctionPointers::function_traits<F>;
            using ReturnType = typename Traits::return_type;
            using ArgsTuple  = typename Traits::args_tuple;
            return make_new_untyped_function_impl<F, ReturnType, ArgsTuple>(
                std::forward<F>(f), std::make_index_sequence<std::tuple_size_v<ArgsTuple>>{}
            );
        }

        template <typename ReturnType, typename... Args>
        static std::unique_ptr<IFunctionPointer> make_unique_untyped(ReturnType (*func)(Args...)) {
            return std::make_unique<FunctionPointers::StaticFunctionPointer<ReturnType, Args...>>(
                func
            );
        }

        template <typename T, typename ReturnType, typename... Args>
        static std::unique_ptr<IFunctionPointer> make_unique_untyped(
            T* object, ReturnType (T::*func)(Args...)
        ) {
            return std::make_unique<
                FunctionPointers::MemberFunctionPointer<T, ReturnType, Args...>>(object, func);
        }

        template <typename F>
        static std::unique_ptr<IFunctionPointer> make_unique_untyped(F&& f) {
            using Traits     = FunctionPointers::function_traits<F>;
            using ReturnType = typename Traits::return_type;
            using ArgsTuple  = typename Traits::args_tuple;
            return make_unique_untyped_function_impl<F, ReturnType, ArgsTuple>(
                std::forward<F>(f), std::make_index_sequence<std::tuple_size_v<ArgsTuple>>{}
            );
        }

        template <typename... Args>
        static FunctionPointers::IFunctionPointerValue** make_new_args(Args&&... args) {
            constexpr auto argsCount = sizeof...(Args);
            return new FunctionPointers::IFunctionPointerValue* [argsCount + 1] {
                new FunctionPointers::FunctionPointerValue<std::decay_t<Args>>(
                    std::forward<Args>(args)
                )...
            };
        }

        template <typename... Args>
        static std::unique_ptr<
            FunctionPointers::IFunctionPointerValue* [],
            FunctionPointers::IFunctionPointerValueArrayDeleter>
        make_unique_args(Args&&... args) {
            return std::unique_ptr<
                FunctionPointers::IFunctionPointerValue*[],
                FunctionPointers::IFunctionPointerValueArrayDeleter>(
                make_new_args(std::forward<Args>(args)...)
            );
        }

        template <typename ReturnType, typename... Args>
        static std::unique_ptr<FunctionPointers::IFunctionPointerValue> invoke(
            ReturnType (*func)(Args...), Args&&... args
        ) {
            return std::unique_ptr<FunctionPointers::IFunctionPointerValue>(
                make_new(func)->invokeWithArgsArray(
                    make_unique_args(std::forward<Args>(args)...).get()
                )
            );
        }

        template <typename T, typename ReturnType, typename... Args>
        static std::unique_ptr<FunctionPointers::IFunctionPointerValue> invoke(
            T* object, ReturnType (T::*func)(Args...), Args&&... args
        ) {
            return std::unique_ptr<FunctionPointers::IFunctionPointerValue>(
                make_new(object, func)
                    ->invokeWithArgsArray(make_unique_args(std::forward<Args>(args)...).get())
            );
        }

        template <typename... Args>
        static std::unique_ptr<FunctionPointers::IFunctionPointerValue> invoke(
            IFunctionPointer* functionPointer, Args&&... args
        ) {
            return std::unique_ptr<FunctionPointers::IFunctionPointerValue>(
                functionPointer->invokeWithArgsArray(
                    make_unique_args(std::forward<Args>(args)...).get()
                )
            );
        }

        template <typename... Args>
        static std::unique_ptr<FunctionPointers::IFunctionPointerValue> invoke(
            std::unique_ptr<IFunctionPointer>& functionPointer, Args&&... args
        ) {
            return std::unique_ptr<FunctionPointers::IFunctionPointerValue>(
                functionPointer->invokeWithArgsArray(
                    make_unique_args(std::forward<Args>(args)...).get()
                )
            );
        }
    };
}
