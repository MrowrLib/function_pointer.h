#pragma once

#include "function_pointers.h"

// Helpers for generating a IFunctionPointer with total type erasure
#define Ifunction_pointer(...) function_pointers::function_pointer::make_unique_untyped(__VA_ARGS__)
#define new_Ifunction_pointer(...) \
    function_pointers::function_pointer::make_new_untyped(__VA_ARGS__)

// Helpers for generating a FunctionPointer<ReturnType, Args...>
#define function_pointer(...) function_pointers::function_pointer::make_unique(__VA_ARGS__)
#define new_function_pointer(...) function_pointers::function_pointer::make_new(__VA_ARGS__)
