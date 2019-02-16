#pragma once

#include <JavaScriptCore/JavaScriptCore.h>
#include <functional>
#include <iostream>
#include <optional>
#include <string>

#include "CJSClass.h"
#include "CJSObject.h"
#include "CJSFunction.h"

namespace cpp_javascriptcore
{

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
