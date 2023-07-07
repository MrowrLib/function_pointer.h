#pragma once

#include <tuple>

namespace function_pointers::FunctionPointers {

    template <typename T>
    struct function_traits;

    template <typename Callable>
    struct function_traits : public function_traits<decltype(&Callable::operator())> {};

    template <typename ClassType, typename ReturnType, typename... Args>
    struct function_traits<ReturnType (ClassType::*)(Args...) const> {
        enum { arity = sizeof...(Args) };

        typedef ReturnType return_type;

        template <size_t i>
        struct arg {
            typedef typename std::tuple_element<i, std::tuple<Args...>>::type type;
        };

        using args_tuple = std::tuple<Args...>;
    };
}
