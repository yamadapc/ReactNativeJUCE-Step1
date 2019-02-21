Toy in-progress attempt of making a React renderer that uses JUCE under the
hood.

- - -

[![Build Status](https://travis-ci.org/yamadapc/ReactNativeJUCE-Step1.svg?branch=master)](https://travis-ci.org/yamadapc/ReactNativeJUCE-Step1)

# cpp_javascriptcore
**cpp_javascriptcore** is a C++ wrapper around JavaScriptCore's C API.

## Basic Usage

### Registering classes
#### High-level
```cpp
// Define a normal C++ class
class Hello
{
public:
    Hello () = default;
    ~Hello () = default;

    std::string sayHello (std::string name)
    {
        return "Hello there " + name + ", have a good day!";
    }
};

// Define what the JavaScript class should be named, and what methods to
// expose on the prototype
auto jsHello = class_<Hello> ("Hello")
               .method ("sayHello", &Hello::sayHello);

// Register the class on the global JavaScript scope
context.registerClass (jsHello);

// Use the class from JavaScript
auto eitherResult = context.evaluateScript (R"(
    const hello = new Hello();
    hello.sayHello('world');
)");

// Unpack the `Either<CJSValue, std::string>` (either result / exception)
std::string result = eitherResult.left ().unsafeGet ().get<std::string> ();

// Magic :)
assert (result == "Hello there world, have a good day!");
```

### Registering functions

#### High-level
**TODO**

`makeCallback`

#### Low-level

```cpp
CJSContext context;
// The callback is registered onto a map and deregistered on destruction.
// You need to keep `callback` around for as long as you want it to be available to JavaScript.
auto callback = context.registerFunction (
  "sayHello",
  [](JSContextRef jsContext, JSValueRef /* functionObject */, JSValueRef /* thisObject */, size_t numArguments, const JSValueRef arguments[], JSValueRef* /* error */) {
    assert (numArguments == 1);

    CJSValue jsStr = {jsContext, arguments[0]};
    assert (jsStr.isString ());

    auto str = jsStr.get<std::string> ();
    std::cout << "JavaScript said: " << str << std::endl;

    return JSValueMakeNull (jsContext);
  }
);

context.evaluateScript ("sayHello ('world')"); // will call the callback
```

### Constructing values
```cpp
CJSContext context;
auto number = CJSValue (context.getContext (), 10);
auto str = CJSValue (context.getContext (), "Hello World");
auto boolean = CJSValue (context.getContext (), false);
// ...
```

### Calling methods
```cpp
CJSContext context;
neither::Either<CJSValue, std::string> eResult = context.evaluateScript (
  "const something = { hello: (n) => n + 10, }; something"
);
CJSValue result = eResult.left ().unsafeGet ();
// CJSValue::get<T>() will also return neither::Either<T, std::string>
CJSObject obj = result.get<CJSObject> ().left ().unsafeGet ();

JSValueRef arguments[1];
arguments[0] = JSValueMakeNumber (context.getContext (), 10.0);

neither::Either<CJSValue, std::string> eHelloResult = obj.callMethod (
  "hello",
  1,
  arguments
);
// ...
```

- - -

## Status

- [ ] Writting C++ wrapper over JavaScriptCore
  - [x] Creating context
  - [x] Registering native C++ functions
  - [ ] Registering native C++ classes
  - [ ] Basic browser-like functions (`setTimeout`, `setInterval`, etc.)
- [ ] JavaScript Bundler
- [ ] Exposing `juce::Component` somehow to JavaScript
