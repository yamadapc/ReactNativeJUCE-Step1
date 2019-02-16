#pragma once

#include <JavaScriptCore/JavaScriptCore.h>
#include <functional>
#include <optional>
#include <string>

namespace cpp_javascriptcore
{
using CbJSArgsReturningJSValue = std::function<JSValueRef (JSObjectRef, size_t, const JSValueRef[])>;

using CbJSArgs = std::function<void(JSObjectRef, size_t, const JSValueRef[])>;

using CbJSArgsReturningJSValueWithContext =
    std::function<JSValueRef (JSContextRef, JSObjectRef, size_t, const JSValueRef[])>;

using CbJSArgsWithContext = std::function<void(JSContextRef, JSObjectRef, size_t, const JSValueRef[])>;

using CbVoid = std::function<void()>;

template <typename CallFn>
JSValueRef callAsFunction (JSContextRef context,
                           JSObjectRef function,
                           JSObjectRef thisObject,
                           size_t numArguments,
                           const JSValueRef arguments[],
                           JSValueRef* error);

template <>
JSValueRef callAsFunction<CbJSArgsReturningJSValue> (JSContextRef context,
                                                     JSObjectRef function,
                                                     JSObjectRef thisObject,
                                                     size_t numArguments,
                                                     const JSValueRef arguments[],
                                                     JSValueRef* error);

template <>
JSValueRef callAsFunction<CbJSArgs> (JSContextRef context,
                                     JSObjectRef function,
                                     JSObjectRef thisObject,
                                     size_t numArguments,
                                     const JSValueRef arguments[],
                                     JSValueRef* error);

template <>
JSValueRef callAsFunction<CbJSArgsReturningJSValueWithContext> (JSContextRef context,
                                                                JSObjectRef function,
                                                                JSObjectRef thisObject,
                                                                size_t numArguments,
                                                                const JSValueRef arguments[],
                                                                JSValueRef* error);

template <>
JSValueRef callAsFunction<CbJSArgsWithContext> (JSContextRef context,
                                                JSObjectRef function,
                                                JSObjectRef thisObject,
                                                size_t numArguments,
                                                const JSValueRef arguments[],
                                                JSValueRef* error);

template <>
JSValueRef callAsFunction<CbVoid> (JSContextRef context,
                                   JSObjectRef function,
                                   JSObjectRef thisObject,
                                   size_t numArguments,
                                   const JSValueRef arguments[],
                                   JSValueRef* error);

template <typename Fn> class CJSFunction
{
public:
    CJSFunction (JSContextRef context_, std::string name, Fn& callback)
        : klass (JSClassCreate (makeFunctionDefinition ())),
          context (context_),
          object (JSObjectMake (context, *klass, NULL)),
          fn (callback)
    {
        auto result = JSObjectSetPrivate (object, &callback);
        assert (result);
    }

    ~CJSFunction ()
    {
    }

    JSValueRef getValue ()
    {
        return object;
    }

    JSObjectRef getObject ()
    {
        return object;
    }

private:
    JSClassDefinition* makeFunctionDefinition ()
    {
        definition = kJSClassDefinitionEmpty;
        definition->callAsFunction = &callAsFunction<Fn>;
        return &*definition;
    }

    std::optional<JSClassDefinition> definition;
    std::optional<JSClassRef> klass;
    JSContextRef context;
    JSObjectRef object;
    Fn fn;
};

} // namespace cpp_javascriptcore
