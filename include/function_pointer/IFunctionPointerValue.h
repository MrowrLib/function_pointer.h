#pragma once

namespace FunctionPointers {

    struct IFunctionPointerValue {
        virtual ~IFunctionPointerValue() = default;
        virtual void* pointer() const    = 0;

        template <typename T>
        T get() const {
            return *static_cast<T*>(pointer());
        }

        template <typename T>
        void set(T value) {
            *static_cast<T*>(pointer()) = value;
        }

        template <typename T>
        T* get_pointer() const {
            return static_cast<T*>(pointer());
        }
    };
}
