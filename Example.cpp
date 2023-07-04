#include <_Log_.h>
#include <function_pointer.h>

void CallMe_Static_VoidReturn_NoArgs() { _Log_("Called CallMe_Static_VoidReturn_NoArgs"); }
int  CallMe_Static_IntReturn_NoArgs() {
    _Log_("Called CallMe_Static_IntReturn_NoArgs");
    return 42;
}
void CallMe_Static_VoidReturn_IntArg(int arg) {
    _Log_("Called CallMe_Static_VoidReturn_IntArg with arg: {}", arg);
}
int CallMe_Static_IntReturn_MultipleArgs(int arg1, bool arg2, float arg3) {
    _Log_("~ Called CallMe_Static_IntReturn_MultipleArgs with args: {}, {}, {}", arg1, arg2, arg3);
    return arg1 + arg3;
}

class SomeClass {
public:
    void CallMe_Member_VoidReturn_NoArgs() { _Log_("Called CallMe_Member_VoidReturn_NoArgs"); }
    int  CallMe_Member_IntReturn_NoArgs() {
        _Log_("Called CallMe_Member_IntReturn_NoArgs");
        return 42;
    }
    void CallMe_Member_VoidReturn_IntArg(int arg) {
        _Log_("Called CallMe_Member_VoidReturn_IntArg with arg: {}", arg);
    }
};

int main() {
    _Log_("STATIC FUNCTION EXAMPLES");

    IFunctionPointer* functionPtr = function_pointer::make_new(CallMe_Static_VoidReturn_NoArgs);
    functionPtr->Invoke();
    delete functionPtr;

    FunctionPointer function = function_pointer::make_unique(CallMe_Static_VoidReturn_NoArgs);
    function->Invoke();

    function = function_pointer(CallMe_Static_VoidReturn_NoArgs);
    function->Invoke();

    function       = function_pointer::make_unique(CallMe_Static_IntReturn_NoArgs);
    auto result    = function->Invoke();
    auto resultInt = result->get<int>();
    _Log_("-> CallMe_Static_IntReturn_NoArgs returned: {}", resultInt);

    function = function_pointer::make_unique(CallMe_Static_VoidReturn_IntArg);

    auto* argsPtr = function_pointer::make_new_args(69);
    function->Invoke(argsPtr);

    auto args = function_pointer::make_unique_args(420);
    function->Invoke(args.get());

    function_pointer::invoke(CallMe_Static_VoidReturn_NoArgs);
    function_pointer::invoke(CallMe_Static_VoidReturn_IntArg, 123);

    auto uniqueResult = function_pointer::invoke(CallMe_Static_IntReturn_NoArgs);
    _Log_("Got int {}", uniqueResult->get<int>());

    function  = function_pointer::make_unique(CallMe_Static_IntReturn_MultipleArgs);
    argsPtr   = function_pointer::make_new_args(69, true, 3.14f);
    result    = function->Invoke(argsPtr);
    resultInt = result->get<int>();
    _Log_("--> -> CallMe_Static_IntReturn_MultipleArgs returned: {}", resultInt);

    uniqueResult = function_pointer::invoke(function.get(), 30, true, 39.0f);
    _Log_("--> --> * Got int {}", uniqueResult->get<int>());

    uniqueResult = function_pointer::invoke(function, 30, true, 39.0f);
    _Log_("--> --> unique Got int {}", uniqueResult->get<int>());

    //
    _Log_("MEMBER FUNCTION EXAMPLES");

    SomeClass someClass;

    functionPtr =
        function_pointer::make_new(&someClass, &SomeClass::CallMe_Member_VoidReturn_NoArgs);
    functionPtr->Invoke();
    delete functionPtr;

    function =
        function_pointer::make_unique(&someClass, &SomeClass::CallMe_Member_VoidReturn_NoArgs);
    function->Invoke();

    function = function_pointer(&someClass, &SomeClass::CallMe_Member_VoidReturn_NoArgs);
    function->Invoke();

    function =
        function_pointer::make_unique(&someClass, &SomeClass::CallMe_Member_IntReturn_NoArgs);
    result    = function->Invoke();
    resultInt = result->get<int>();
    _Log_("-> CallMe_Member_IntReturn_NoArgs returned: {}", resultInt);

    function =
        function_pointer::make_unique(&someClass, &SomeClass::CallMe_Member_VoidReturn_IntArg);

    argsPtr = function_pointer::make_new_args(69);
    function->Invoke(argsPtr);

    args = function_pointer::make_unique_args(420);
    function->Invoke(args.get());

    function_pointer::invoke(&someClass, &SomeClass::CallMe_Member_VoidReturn_NoArgs);
    function_pointer::invoke(&someClass, &SomeClass::CallMe_Member_VoidReturn_IntArg, 123);

    uniqueResult = function_pointer::invoke(&someClass, &SomeClass::CallMe_Member_IntReturn_NoArgs);
    _Log_("Got int {}", uniqueResult->get<int>());

    //
    _Log_("And you can also use std::function");

    functionPtr = new_function_pointer([]() { _Log_("Called a lambda! (pointer)"); });
    functionPtr->Invoke();

    function = function_pointer([]() { _Log_("Called a lambda! (unique)"); });
    function->Invoke();

    int captureMe = 123;
    function      = function_pointer([captureMe]() {
        _Log_("Called a lambda! (unique) with capture: {}", captureMe);
    });
    function_pointer::invoke(function);

    function = function_pointer([captureMe](int arg) {
        _Log_("Called a lambda! (unique) with arg: {} and captured: {}", arg, captureMe);
    });
    function_pointer::invoke(function, 69);
}
