# `#include <function_pointer.h>`

```cpp
#include <function_pointer.h>

void CallMe(int input) { /* ... */ }

class Example {
    void CallMe(int input) { /* ... */ }    
}

void Example() {
    // Store a static function pointer to call later
    IFunctionPointer* functionPointer = function_pointer(CallMe);

    // Store a member function pointer to call later
    Example example;
    IFunctionPointer* memberFunctionPointer = function_pointer(&example, &Example::CallMe);

    // Call the function pointers
    functionPointer->Invoke(123);
    memberFunctionPointer->Invoke(123);
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
            "baseline": "6f0bae92873e103c18dd1d4f0a9ea432a4bb942e",
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
// Example of a static function to call
void CallMe(int input) { /* ... */ }

// Example of a class with a member function to call
class Example {
    void CallMe(int input) { /* ... */ }    
}
```

```cpp
#include <function_pointer.h>

// You can store a pointer to a static function like this:
FunctionPointer functionPointer = function_pointer(CallMe);

// This is shorthand for:
std::unique_ptr<IFunctionPointer> functionPointer =
  function_pointer::make_unique(CallMe);

// If you want a raw pointer:
IFunctionPointer* functionPointer = function_pointer::make_new(CallMe);

// And you can invoke the function using the Invoke method:
functionPointer->Invoke(123);
```

```cpp
// You can store a pointer to a member function like this:
Example example;
FunctionPointer memberFunctionPointer = function_pointer(&example, &Example::CallMe);

// This is shorthand for:
std::unique_ptr<IFunctionPointer> memberFunctionPointer =
  function_pointer::make_unique(&example, &Example::CallMe);

// If you want a raw pointer:
IFunctionPointer* memberFunctionPointer =
  function_pointer::make_new(&example, &Example::CallMe);

// And you can invoke the function using the Invoke method:
memberFunctionPointer->Invoke(123);
```

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
