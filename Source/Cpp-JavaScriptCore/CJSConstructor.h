#pragma once

#include <JavaScriptCore/JavaScriptCore.h>
#include <functional>
#include <unordered_map>

namespace cpp_javascriptcore
{

class CJSConstructor
{
public:
    using Constructor = std::function<JSObjectRef (JSContextRef, JSObjectRef, size_t, const JSValueRef[], JSValueRef*)>;

private:
    static JSObjectRef runConstructor (JSContextRef context,
                                       JSObjectRef constructor,
                                       size_t numArguments,
                                       const JSValueRef arguments[],
                                       JSValueRef* error);

    static std::unordered_map<JSObjectRef, Constructor> constructorMap;

public:
    CJSConstructor (JSContextRef context_, JSClassRef klass_, Constructor& constructor);

    ~CJSConstructor ();

    JSClassRef getClass ()
    {
        return klass;
    }

    JSObjectRef getConstructor ()
    {
        return jsConstructor;
    }

private:
    JSContextRef context;
    JSClassRef klass;
    JSObjectRef jsConstructor;
};

} // namespace cpp_javascriptcore
