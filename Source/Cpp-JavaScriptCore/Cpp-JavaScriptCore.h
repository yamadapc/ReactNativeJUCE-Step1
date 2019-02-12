#pragma once

#include <JavaScriptCore/JavaScriptCore.h>
#include <functional>
#include <iostream>
#include <optional>
#include <string>

namespace cpp_javascriptcore
{
struct UndefinedLiteral
{
};
struct NullLiteral
{
};

JSStringRef getJSStringRefFromString (const std::string& str);

class CJSClass
{
public:
    CJSClass (JSClassRef klass_) : klass (klass_)
    {
    }
    ~CJSClass ()
    {
    }

    JSClassRef getClass ()
    {
        return klass;
    }

private:
    JSClassRef klass;
};

class CJSObject
{
public:
    CJSObject ()
    {
    }

    CJSObject (JSContextRef context_, JSObjectRef object_) : context (context_), object (object_)
    {
    }

    CJSObject (JSContextRef context, CJSClass klass, void* data)
        : CJSObject (context, JSObjectMake (context, klass.getClass (), data))
    {
    }

    CJSObject (JSContextRef context, std::string name, JSObjectCallAsFunctionCallback callback)
        : CJSObject (context, JSObjectMakeFunctionWithCallback (context, getJSStringRefFromString (name), callback))
    {
    }

    ~CJSObject ()
    {
    }

    void setProperty (std::string propertyName, JSValueRef value)
    {
        JSObjectSetProperty (context, object, getJSStringRefFromString (propertyName), value, NULL, NULL);
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
    JSContextRef context;
    JSObjectRef object;
};

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
