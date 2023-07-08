#pragma once

#include <memory>

#include "FunctionalFunctionPointer.h"
#include "MemberFunctionPointer.h"
#include "StaticFunctionPointer.h"

namespace FunctionPointers {

    template <typename Func>
    struct FunctionPointer;

    template <typename ReturnType, typename... Args>
    struct FunctionPointer<ReturnType(Args...)> : public IFunctionPointer<ReturnType(Args...)> {
        bool _destroyFunctionPointerBaseOnDestruction = true;
        std::unique_ptr<IFunctionPointer<ReturnType(Args...)>> _functionPointer;

    public:
        ~FunctionPointer() {
            if (!destroys_function_pointer_base_on_destruction())
                auto* ptr = _functionPointer.release();
        }

        FunctionPointer(ReturnType (*func)(Args...)) {
            _functionPointer =
                std::make_unique<FunctionPointers::StaticFunctionPointer<ReturnType(Args...)>>(func
                );
        }

        FunctionPointer(std::function<ReturnType(Args...)> func) {
            _functionPointer =
                std::make_unique<FunctionPointers::FunctionalFunctionPointer<ReturnType(Args...)>>(
                    func
                );
        }

        template <typename T>
        FunctionPointer(T* instance, ReturnType (T::*func)(Args...)) {
            _functionPointer =
                std::make_unique<FunctionPointers::MemberFunctionPointer<T, ReturnType, Args...>>(
                    instance, func
                );
        }

        FunctionPointers::IFunctionPointerValue* invokeWithArgsArray(
            FunctionPointers::IFunctionPointerValue** args
        ) override {
            return _functionPointer->invokeWithArgsArray(args);
        }

        virtual IFunctionPointer<ReturnType(Args...)>* inner_function_pointer() const {
            return _functionPointer.get();
        }

        virtual void set_destroys_function_pointer_base_on_destruction(bool value) {
            _destroyFunctionPointerBaseOnDestruction = value;
        }

        virtual void do_not_destroy_function_pointer() {
            set_destroys_function_pointer_base_on_destruction(false);
        }

        virtual bool destroys_function_pointer_base_on_destruction() const {
            return _destroyFunctionPointerBaseOnDestruction;
        }
    };
}