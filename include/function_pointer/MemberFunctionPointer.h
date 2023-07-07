#pragma once

#include <utility>

#include "FunctionPointerValue.h"
#include "IFunctionPointer.h"

namespace FunctionPointers {

    template <typename T, typename ReturnType, typename... Args>
    class MemberFunctionPointer : public IFunctionPointer {
        ReturnType (T::*_func)(Args...);
        T* _instance;

        template <std::size_t... I>
        IFunctionPointerValue* InvokeAndReturnImpl(
            std::index_sequence<I...>, IFunctionPointerValue** args
        ) {
            return new FunctionPointerValue<ReturnType>((_instance->*_func)(
                static_cast<FunctionPointerValue<
                    typename std::tuple_element<I, std::tuple<Args...>>::type>*>(args[I])
                    ->GetValue()...
            ));
        }

        template <std::size_t... I>
        void InvokeImpl(std::index_sequence<I...>, IFunctionPointerValue** args) {
            (_instance->*_func)(
                static_cast<FunctionPointerValue<
                    typename std::tuple_element<I, std::tuple<Args...>>::type>*>(args[I])
                    ->GetValue()...
            );
        }

    public:
        MemberFunctionPointer(T* instance, ReturnType (T::*func)(Args...))
            : _func(func), _instance(instance) {}

        IFunctionPointerValue* InvokeWithArgsArray(IFunctionPointerValue** args) override {
            if constexpr (!std::is_same<ReturnType, void>::value) {
                return InvokeAndReturnImpl(std::index_sequence_for<Args...>{}, args);
            } else {
                InvokeImpl(std::index_sequence_for<Args...>{}, args);
                return nullptr;
            }
        }
    };
}