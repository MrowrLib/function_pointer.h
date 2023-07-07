#pragma once

#include <memory>

#include "IFunctionPointer.h"


namespace FunctionPointers {

    using FunctionPointer = std::unique_ptr<IFunctionPointer>;
}
