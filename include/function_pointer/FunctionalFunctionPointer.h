#pragma once

#include <functional>
#include <memory>
#include <utility>
#include <vector>

#include "FunctionPointerValue.h"
#include "IFunctionPointer.h"

namespace FunctionPointers {

    template <typename ReturnType, typename... Args>
    class FunctionalFunctionPointer : public IFunctionPointer {
        std::function<ReturnType(Args...)> _func;

        template <std::size_t... I>
        IFunctionPointerValue* InvokeAndReturnImpl(
            std::index_sequence<I...>, IFunctionPointerValue** args
        ) {
            return new FunctionPointerValue<ReturnType>(
                _func(static_cast<FunctionPointerValue<
                          typename std::tuple_element<I, std::tuple<Args...>>::type>*>(args[I])
                          ->GetValue()...)
            );
        }

        template <std::size_t... I>
        void InvokeImpl(std::index_sequence<I...>, IFunctionPointerValue** args) {
            _func(static_cast<FunctionPointerValue<
                      typename std::tuple_element<I, std::tuple<Args...>>::type>*>(args[I])
                      ->GetValue()...);
        }

    public:
        FunctionalFunctionPointer(std::function<ReturnType(Args...)> func) : _func(func) {}

        IFunctionPointerValue* Invoke(IFunctionPointerValue** args) override {
            if constexpr (!std::is_same<ReturnType, void>::value) {
                return InvokeAndReturnImpl(std::index_sequence_for<Args...>{}, args);
            } else {
                InvokeImpl(std::index_sequence_for<Args...>{}, args);
                return nullptr;
            }
        }
    };
}
