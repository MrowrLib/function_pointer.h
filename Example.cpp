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

void CallThisWithAFunction(FunctionPointer<void(int)> function) {
    _Log_("Called CallThisWithAFunction() with function");
    function.invoke(123);

    _Log_("And we can work with it after type erasure too");
    IFunctionPointer* functionPtr = function.inner_function_pointer();
    function_pointer::invoke(functionPtr, 456);

    _Log_("The FunctionPointer is itself a IFunctionPointer too tho");
    IFunctionPointer* theFunctionPtr = &function;
    function_pointer::invoke(theFunctionPtr, 789);
    _Log_("...");
}

int main() {
    CallThisWithAFunction(FunctionPointer<void(int)>(CallMe_Static_VoidReturn_IntArg));
    CallThisWithAFunction(function_pointer(CallMe_Static_VoidReturn_IntArg));

    // These work great!

    // FunctionPointer(CallMe_Static_VoidReturn_NoArgs).invoke();

    // SomeClass someClass;
    // FunctionPointer(&someClass, &SomeClass::CallMe_Member_VoidReturn_NoArgs).invoke();

    // auto x = new_function_pointer([]() { _Log_("Hiiiiii from llllambda!!!"); });
    // x->invoke();

    // Nothing below here works!

    // auto lam = std::function([]() { _Log_("Called lambda"); });
    // auto ptr = FunctionPointer<void>(lam);
    // ptr.invoke();

    // _Log_("STATIC FUNCTION EXAMPLES");

    // IFunctionPointer* functionPtr =
    //     function_pointer::make_new_untyped(CallMe_Static_VoidReturn_NoArgs);
    // functionPtr->invoke();
    // delete functionPtr;

    // FunctionPointer<void, int>* functionPointer1 =
    //     new FunctionPointer<void, int>(CallMe_Static_VoidReturn_IntArg);
    // functionPointer1->invoke(123);

    // _Log_("...");

    // function_pointer(CallMe_Static_VoidReturn_NoArgs);

    // FunctionPointer function = function_pointer::make_unique(CallMe_Static_VoidReturn_NoArgs);
    // function->invoke();

    // function = function_pointer(CallMe_Static_VoidReturn_NoArgs);
    // function->invoke();

    // function       = function_pointer::make_unique(CallMe_Static_IntReturn_NoArgs);
    // auto result    = function->invoke();
    // auto resultInt = result->get<int>();
    // _Log_("-> CallMe_Static_IntReturn_NoArgs returned: {}", resultInt);

    // function = function_pointer::make_unique(CallMe_Static_VoidReturn_IntArg);

    // auto* argsPtr = function_pointer::make_new_args(69);
    // function->invokeWithArgsArray(argsPtr);

    // auto args = function_pointer::make_unique_args(420);
    // function->invokeWithArgsArray(args.get());

    // function_pointer::invoke(CallMe_Static_VoidReturn_NoArgs);
    // function_pointer::invoke(CallMe_Static_VoidReturn_IntArg, 123);

    // auto uniqueResult = function_pointer::invoke(CallMe_Static_IntReturn_NoArgs);
    // _Log_("Got int {}", uniqueResult->get<int>());

    // function  = function_pointer::make_unique(CallMe_Static_IntReturn_MultipleArgs);
    // argsPtr   = function_pointer::make_new_args(69, true, 3.14f);
    // result    = function->invokeWithArgsArray(argsPtr);
    // resultInt = result->get<int>();
    // _Log_("--> -> CallMe_Static_IntReturn_MultipleArgs returned: {}", resultInt);

    // uniqueResult = function_pointer::invoke(function.get(), 30, true, 39.0f);
    // _Log_("--> --> * Got int {}", uniqueResult->get<int>());

    // uniqueResult = function_pointer::invoke(function, 30, true, 39.0f);
    // _Log_("--> --> unique Got int {}", uniqueResult->get<int>());

    // //
    // _Log_("MEMBER FUNCTION EXAMPLES");

    // SomeClass someClass;

    // functionPtr =
    //     function_pointer::make_new(&someClass, &SomeClass::CallMe_Member_VoidReturn_NoArgs);
    // functionPtr->invoke();
    // delete functionPtr;

    // function =
    //     function_pointer::make_unique(&someClass, &SomeClass::CallMe_Member_VoidReturn_NoArgs);
    // function->invoke();

    // function = function_pointer(&someClass, &SomeClass::CallMe_Member_VoidReturn_NoArgs);
    // function->invoke();

    // function =
    //     function_pointer::make_unique(&someClass, &SomeClass::CallMe_Member_IntReturn_NoArgs);
    // result    = function->invoke();
    // resultInt = result->get<int>();
    // _Log_("-> CallMe_Member_IntReturn_NoArgs returned: {}", resultInt);

    // function =
    //     function_pointer::make_unique(&someClass, &SomeClass::CallMe_Member_VoidReturn_IntArg);

    // argsPtr = function_pointer::make_new_args(69);
    // function->invokeWithArgsArray(argsPtr);

    // args = function_pointer::make_unique_args(420);
    // function->invokeWithArgsArray(args.get());

    // function_pointer::invoke(&someClass, &SomeClass::CallMe_Member_VoidReturn_NoArgs);
    // function_pointer::invoke(&someClass, &SomeClass::CallMe_Member_VoidReturn_IntArg, 123);

    // uniqueResult = function_pointer::invoke(&someClass,
    // &SomeClass::CallMe_Member_IntReturn_NoArgs); _Log_("Got int {}", uniqueResult->get<int>());

    // //
    // _Log_("And you can also use std::function");

    // functionPtr = new_function_pointer([]() { _Log_("Called a lambda! (pointer)"); });
    // functionPtr->invoke();

    // function = function_pointer([]() { _Log_("Called a lambda! (unique)"); });
    // function->invoke();

    // int captureMe = 123;
    // function      = function_pointer([captureMe]() {
    //     _Log_("Called a lambda! (unique) with capture: {}", captureMe);
    // });
    // function_pointer::invoke(function);

    // function = function_pointer([captureMe](int arg) {
    //     _Log_("Called a lambda! (unique) with arg: {} and captured: {}", arg, captureMe);
    // });
    // function_pointer::invoke(function, 69);
}
