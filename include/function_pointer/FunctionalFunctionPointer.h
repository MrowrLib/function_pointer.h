#pragma once

#include <functional>
#include <memory>
#include <utility>
#include <vector>

#include "FunctionPointerValue.h"
#include "IFunctionPointer.h"

namespace function_pointers::FunctionPointers {

    template <typename Func>
    class FunctionalFunctionPointer;

    template <typename ReturnType, typename... Args>
    class FunctionalFunctionPointer<ReturnType(Args...)> : public IFunctionPointer {
        std::function<ReturnType(Args...)> _func;

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
        FunctionalFunctionPointer(std::function<ReturnType(Args...)> func) : _func(func) {}

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
