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

    auto* functionPtr = function_pointer::make_new(CallMe_Static_VoidReturn_NoArgs);
    functionPtr->Invoke();
    delete functionPtr;

    auto function = function_pointer::make_unique(CallMe_Static_VoidReturn_NoArgs);
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
}