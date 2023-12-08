#pragma once

#include <utility>

#include "FunctionPointerValue.h"
#include "IMemberFunctionPointer.h"

namespace FunctionPointers {

    template <typename T, typename ReturnType, typename... Args>
    class MemberFunctionPointer : public IMemberFunctionPointer<T, ReturnType, Args...> {
        ReturnType (T::*_func)(Args...);
        T*         _instance;

        template <std::size_t... I>
        IFunctionPointerValue* invokeAndReturnImpl(
            std::index_sequence<I...>, IFunctionPointerValue** args
        ) const {
            return new FunctionPointerValue<ReturnType>((_instance->*_func)(
                static_cast<FunctionPointerValue<
                    typename std::tuple_element<I, std::tuple<Args...>>::type>*>(args[I])
                    ->GetValue()...
            ));
        }

        template <std::size_t... I>
        void invokeImpl(std::index_sequence<I...>, IFunctionPointerValue** args) const {
            (_instance->*_func)(
                static_cast<FunctionPointerValue<
                    typename std::tuple_element<I, std::tuple<Args...>>::type>*>(args[I])
                    ->GetValue()...
            );
        }

    public:
        MemberFunctionPointer(T* instance, ReturnType (T::*func)(Args...))
            : _func(func), _instance(instance) {}

        IFunctionPointerValue* invokeWithArgsArray(IFunctionPointerValue** args) const override {
            if constexpr (!std::is_same<ReturnType, void>::value) {
                return invokeAndReturnImpl(std::index_sequence_for<Args...>{}, args);
            } else {
                invokeImpl(std::index_sequence_for<Args...>{}, args);
                return nullptr;
            }
        }
    };
}