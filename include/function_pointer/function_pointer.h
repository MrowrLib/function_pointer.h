#pragma once

#include "function_pointers.h"

// Helpers for generating a IFunctionPointerBase with total type erasure
#define new_Ifunction_pointer(...) FunctionPointers::function_pointer::make_new_untyped(__VA_ARGS__)
#define unique_Ifunction_pointer(...) \
    FunctionPointers::function_pointer::make_unique_untyped(__VA_ARGS__)
#define Ifunction_pointer(...) new_Ifunction_pointer(__VA_ARGS__)

// Helpers for generating a FunctionPointer<ReturnType(Args...)>
#define function_pointer(...) FunctionPointers::function_pointer::make(__VA_ARGS__)
#define new_function_pointer(...) FunctionPointers::function_pointer::make_new(__VA_ARGS__)
#define unique_function_pointer(...) FunctionPointers::function_pointer::make_unique(__VA_ARGS__)
