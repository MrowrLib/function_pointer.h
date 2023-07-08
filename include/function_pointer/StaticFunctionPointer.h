#pragma once

#include <memory>
#include <utility>
#include <vector>

#include "FunctionPointerValue.h"
#include "ITypedFunctionPointer.h"

namespace function_pointers::FunctionPointers {

    template <typename Func>
    class StaticFunctionPointer;

    template <typename ReturnType, typename... Args>
    class StaticFunctionPointer<ReturnType(Args...)>
        : public ITypedFunctionPointer<ReturnType(Args...)> {
        ReturnType (*_func)(Args...);

        template <std::size_t... I>
        IFunctionPointerValue* invokeAndReturnImpl(
            std::index_sequence<I...>, IFunctionPointerValue** args
        ) {
            return new FunctionPointerValue<ReturnType>(
                _func(static_cast<FunctionPointerValue<
                          typename std::tuple_element<I, std::tuple<Args...>>::type>*>(args[I])
                          ->GetValue()...)
            );
        }

        template <std::size_t... I>
        void invokeImpl(std::index_sequence<I...>, IFunctionPointerValue** args) {
            _func(static_cast<FunctionPointerValue<
                      typename std::tuple_element<I, std::tuple<Args...>>::type>*>(args[I])
                      ->GetValue()...);
        }

    public:
        StaticFunctionPointer(ReturnType (*func)(Args...)) : _func(func) {}

        IFunctionPointerValue* invokeWithArgsArray(IFunctionPointerValue** args) override {
            if constexpr (!std::is_same<ReturnType, void>::value) {
                return invokeAndReturnImpl(std::index_sequence_for<Args...>{}, args);
            } else {
                invokeImpl(std::index_sequence_for<Args...>{}, args);
                return nullptr;
            }
        }
    };
}
