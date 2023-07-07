#pragma once

#include <utility>

#include "FunctionPointerValue.h"
#include "IFunctionPointer.h"

namespace function_pointers::FunctionPointers {

    template <typename Func>
    class MemberFunctionPointer;

    template <typename T, typename ReturnType, typename... Args>
    class MemberFunctionPointer<ReturnType(T, Args...)> : public IFunctionPointer {
        ReturnType (T::*_func)(Args...);
        T* _instance;

        template <std::size_t... I>
        IFunctionPointerValue* invokeAndReturnImpl(
            std::index_sequence<I...>, IFunctionPointerValue** args
        ) {
            return new FunctionPointerValue<ReturnType>((_instance->*_func)(
                static_cast<FunctionPointerValue<
                    typename std::tuple_element<I, std::tuple<Args...>>::type>*>(args[I])
                    ->GetValue()...
            ));
        }

        template <std::size_t... I>
        void invokeImpl(std::index_sequence<I...>, IFunctionPointerValue** args) {
            (_instance->*_func)(
                static_cast<FunctionPointerValue<
                    typename std::tuple_element<I, std::tuple<Args...>>::type>*>(args[I])
                    ->GetValue()...
            );
        }

    public:
        MemberFunctionPointer(T* instance, ReturnType (T::*func)(Args...))
            : _func(func), _instance(instance) {}

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