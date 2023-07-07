#pragma once

#include <memory>

#include "IFunctionPointerCore.h"

namespace FunctionPointers {

    using FunctionPointer = std::unique_ptr<IFunctionPointerCore>;
}
