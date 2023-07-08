#pragma once

#include <vector>

#include "IFunctionPointerValue.h"

namespace FunctionPointers {

    struct IFunctionPointerValueArrayDeleter {
        void operator()(IFunctionPointerValue** array) const {
            if (array == nullptr) return;
            for (std::size_t i = 0; array[i] != nullptr; ++i) delete array[i];
            delete[] array;
        }
    };
}
