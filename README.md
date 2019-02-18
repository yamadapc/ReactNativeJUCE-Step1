Toy in-progress attempt of making a React renderer that uses JUCE under the
hood.

- - -

# cpp_javascriptcore
**cpp_javascriptcore** is a C++ wrapper around JavaScriptCore's C API.

## Basic Usage
### Registering functions

```cpp
CJSContext context;

context.registerFunction (
  "sayHello",
  [](JSContextRef jsContext, JSValueRef /* functionObject */, JSValueRef /* thisObject */, size_t numArguments, const JSValueRef arguments[], JSValueRef* /* error */) {
    assert (numArguments == 1);
    CJSValue jsStr = {jsContext, arguments[0]};
    assert (jsStr.isString ());
    auto str = jsStr.get<std::string> ();
    std::cout << "JavaScript said: " << str << std::endl;
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
