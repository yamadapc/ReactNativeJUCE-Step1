#pragma once

#include <JavaScriptCore/JavaScriptCore.h>
#include <functional>
#include <optional>
#include <string>
#include <unordered_map>

#include <iostream>

#include "CJSUtil.h"

namespace cpp_javascriptcore
{

using CbJSArgsReturningJSValueWithContext =
    std::function<JSValueRef (JSContextRef, JSObjectRef, size_t, const JSValueRef[])>;

class CJSFunction
{
public:
    using Callback =
        std::function<JSValueRef (JSContextRef, JSObjectRef, JSObjectRef, size_t, const JSValueRef[], JSValueRef*)>;

private:
    static JSValueRef runCallback (JSContextRef context,
                                   JSObjectRef function,
                                   JSObjectRef thisObject,
                                   size_t numArguments,
                                   const JSValueRef arguments[],
                                   JSValueRef* error);

    static std::unordered_map<JSObjectRef, Callback> callbackMap;

public:
    CJSFunction (JSContextRef context_, const std::string& name, Callback& callback);

    ~CJSFunction ();

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

} // namespace cpp_javascriptcore
