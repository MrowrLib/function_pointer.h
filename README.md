# `#include <function_pointer.h>`

```cpp
#include <function_pointer.h>

void CallMe(int input) { /* ... */ }

class Example {
    void CallMe(int input) { /* ... */ }    
}

void PassMeAFunction(FunctionPointer<void(int)> fn) {
    // Invoke function pointer
    fn.invoke(123);

    // Store any kind of FunctionPointer<*> in a container (with type erasure)
    std::vector<IFunctionPointer*> _functionPointers;
    _functionPointers.push_back(fn);
}

void Example() {
    // Store a static function pointer to call later
    auto functionPointer = function_pointer(CallMe);

    // Store a member function pointer to call later
    Example example;
    auto memberFunctionPointer = function_pointer(&example, &Example::CallMe);

    // Or a lambda (with or without captures)
    auto lambdaFunctionPointer = function_pointer([]() { /* ... */ });

    // Call the function pointers
    functionPointer.invoke(123);
    memberFunctionPointer.invoke(123);
    lambdaFunctionPointer.invoke(123);

    // Implicit convestions to FunctionPointer:
    PassMeAFunction(CallMe);
    PassMeAFunction(&example, &Example::CallMe);
    PassMeAFunction([]() { /* ... */ });
}
```

## What?

Easily store and call function pointers and member function pointers in C++

## Installation

### xmake

#### `xmake.lua`

```lua
add_repositories("MrowrLib https://github.com/MrowrLib/Packages.git")
add_requires("function_pointer")

target("Example")
    add_packages("function_pointer")
```

### vcpkg

#### `CMakeLists.txt`

```cmake
add_executable(Example main.cpp)

# Find function_pointer and link it to your target
find_package(function_pointer CONFIG REQUIRED)
target_link_libraries(Example PRIVATE function_pointer::function_pointer)
```

#### `vcpkg.json`

```json
{
    "dependencies": ["mrowr-function-pointer"]
}
```

#### `vcpkg-configuration.json`

```json
{
    "default-registry": {
        "kind": "git",
        "repository": "https://github.com/microsoft/vcpkg.git",
        "baseline": "95252eadd63118201b0d0df0b4360fa613f0de84"
    },
    "registries": [
        {
            "kind": "git",
            "repository": "https://github.com/MrowrLib/Packages.git",
            "baseline": "83e936a2270e029ad29b109bdbda087bc95fee5e",
            "packages": ["mrowr-function-pointer"]
        }
    ]
}
```

> _Update the default-registry baseline to the latest commit from https://github.com/microsoft/vcpkg_  
> _Update the MrowrLib/Packages baseline to the latest commit from https://github.com/MrowrLib/Packages_

## Why?

I needed to store both:
- function pointers
- member function pointers (_from different types_)

And I needed a pointer I could pass around to refer to the function pointer and invoke it.

So I made this.

> _Note: I need this for scenarios when I'm working with true function pointers._  
> _If you have the option of using `std::function` instead, highly recommend using that!_

## How?

```cpp
#include <function_pointer.h>

void CallMe(int input) { /* ... */ }

class Example {
    void CallMe(int input) { /* ... */ }    
};

void Example() {
    // Store a static function pointer to call later
    auto functionPointer = function_pointer(CallMe);

    // Store a member function pointer to call later
    Example example;
    auto memberFunctionPointer = function_pointer(&example, &Example::CallMe);

    // Or a lambda (with or without captures)
    auto lambdaFunctionPointer = function_pointer([]() { /* ... */ });

    // Call the function pointers
    functionPointer.invoke(123);
    memberFunctionPointer.invoke(123);
    lambdaFunctionPointer.invoke(123);
}
```

### `FunctionPointer<ReturnType(Args...)>`

If you want to specify required return type and arguments for a function pointer, you can use the `FunctionPointer` template.

```cpp
// Using the `function_pointer` helper function
// which returns std::unique_ptr<FunctionPointer<ReturnType(Args...)>>
auto functionPointer = function_pointer(CallMe); // Static function pointer
auto functionPointer = function_pointer<this, &Example::CallMe); // Member function pointer
auto functionPointer = function_pointer([]() { /* ... */ }); // Lambda or std::function

// There is also a helper which returns a raw pointer
auto* functionPointer = new_function_pointer(CallMe); // Static function pointer
auto* functionPointer = new_function_pointer<this, &Example::CallMe); // Member function pointer
auto* functionPointer = new_function_pointer([]() { /* ... */ }); // Lambda or std::function
```

If you declare a function which accepts a `FunctionPointer` as an argument, you must specify the return type and arguments.

```cpp
void MyFunction(FunctionPointer<void, int> functionPointer) {
    functionPointer.invoke(123);
}
```

### `IFunctionPointer`

If you want to store a function pointer without specifying the return type or arguments, you can use the `IFunctionPointer` interface.

> This template uses type erasure, allowing you to easily store instances without knowing the return type or arguments.

```cpp
// Using the `function_pointer` helper function
// which returns std::unique_ptr<IFunctionPointer>
auto functionPointer = Ifunction_pointer(CallMe); // Static function pointer
auto functionPointer = Ifunction_pointer<this, &Example::CallMe); // Member function pointer
auto functionPointer = Ifunction_pointer([]() { /* ... */ }); // Lambda or std::function

// There is also a helper which returns a raw pointer
IFunctionPointer* functionPointer = new_ifunction_pointer(CallMe); // Static function pointer
IFunctionPointer* functionPointer = new_ifunction_pointer<this, &Example::CallMe); // Member function pointer
IFunctionPointer* functionPointer = new_ifunction_pointer([]() { /* ... */ }); // Lambda or std::function
```

`IFunctionPointer*` is the reason why this library exists.

You can store a `IFunctionPointer*` and invoke it later, without knowing the return type or arguments.

You can easily store various `IFunctionPointer*` of different types in the same container.

## License

Use however, no attribution required.

```
BSD Zero Clause License (SPDX: 0BSD)

Permission to use, copy, modify, and/or distribute this software for any purpose
with or without fee is hereby granted.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND
FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS
OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF
THIS SOFTWARE.
```
