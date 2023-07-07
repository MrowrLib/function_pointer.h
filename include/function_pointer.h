#pragma once

#include "function_pointer/function_pointer.h"

typedef FunctionPointers::function_pointer     function_pointer;
typedef FunctionPointers::IFunctionPointerCore IFunctionPointerCore;
typedef FunctionPointers::FunctionPointer      FunctionPointer;

#define function_pointer(...) function_pointer::make_unique(__VA_ARGS__)
#define new_function_pointer(...) function_pointer::make_new(__VA_ARGS__)
