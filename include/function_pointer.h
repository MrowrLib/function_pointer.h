#pragma once

#include "function_pointer/function_pointer.h"

typedef FunctionPointers::function_pointer function_pointer;
typedef FunctionPointers::IFunctionPointer IFunctionPointer;
typedef FunctionPointers::FunctionPointer  FunctionPointer;

#define function_pointer(...) function_pointer::make_unique(__VA_ARGS__)
