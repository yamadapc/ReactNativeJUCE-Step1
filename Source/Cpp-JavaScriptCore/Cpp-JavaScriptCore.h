#pragma once

#include <JavaScriptCore/JavaScriptCore.h>
#include <functional>
#include <iostream>
#include <optional>
#include <string>

#include "CJSClass.h"
#include "CJSObject.h"

namespace cpp_javascriptcore
{
struct UndefinedLiteral
{
};
struct NullLiteral
{
};

JSStringRef getJSStringRefFromString (const std::string& str);

template <typename CallFn>
JSValueRef callAsFunction (JSContextRef context,
                           JSObjectRef function,
                           JSObjectRef thisObject,
                           size_t numArguments,
                           const JSValueRef arguments[],
                           JSValueRef* error);

template <>
JSValueRef
callAsFunction<std::function<JSValueRef (JSObjectRef, size_t, const JSValueRef[])>> (JSContextRef context,
                                                                                     JSObjectRef function,
                                                                                     JSObjectRef thisObject,
                                                                                     size_t numArguments,
                                                                                     const JSValueRef arguments[],
                                                                                     JSValueRef* error);

template <>
JSValueRef callAsFunction<std::function<void(JSObjectRef, size_t, const JSValueRef[])>> (JSContextRef context,
                                                                                         JSObjectRef function,
                                                                                         JSObjectRef thisObject,
                                                                                         size_t numArguments,
                                                                                         const JSValueRef arguments[],
                                                                                         JSValueRef* error);

template <>
JSValueRef callAsFunction<std::function<void()>> (JSContextRef context,
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

// class (*JSObjectCallAsFunctionCallback) (JSContextRef ctx, JSObjectRef
// function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef
// arguments[], JSValueRef* exception);

class CJSValue
{
public:
    enum class CJSType
    {
        Undefined,
        Null,
        Boolean,
        Number,
        String,
        Object,
    };

    CJSValue (JSContextRef context_, JSValueRef value_);
    ~CJSValue ();

    CJSType getType ();
    bool isUndefined ();
    bool isNull ();
    bool isBoolean ();
    bool isNumber ();
    bool isString ();
    bool isObject ();

    JSValueRef getValue ();

private:
    static CJSType jsTypeToCJSType (JSType type);

    JSContextRef context;
    JSValueRef value;
};

class CJSContext
{
public:
    CJSContext ();
    ~CJSContext ();

    CJSValue evaluateScript (std::string script);
    CJSObject getGlobalObject ();

    JSGlobalContextRef getContext ();

    void registerFunction (std::string name,
                           std::function<JSValueRef (JSObjectRef, size_t, const JSValueRef[])> callback)
    {
        auto global = getGlobalObject ();
        auto jsCallback = CJSFunction (context, name, callback);
        global.setProperty (name, jsCallback.getValue ());
    }

    void registerFunction (std::string name, std::function<void(JSObjectRef, size_t, const JSValueRef[])> callback)
    {
        auto global = getGlobalObject ();
        auto jsCallback = CJSFunction (context, name, callback);
        global.setProperty (name, jsCallback.getValue ());
    }

    void registerFunction (std::string name, std::function<void()> callback)
    {
        auto global = getGlobalObject ();
        auto jsCallback = CJSFunction (context, name, callback);
        global.setProperty (name, jsCallback.getValue ());
    }

private:
    JSGlobalContextRef context;
};
} // namespace cpp_javascriptcore
