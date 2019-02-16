#pragma once

#include <JavaScriptCore/JavaScriptCore.h>
#include <functional>
#include <iostream>
#include <optional>
#include <string>

#include "CJSClass.h"
#include "CJSFunction.h"
#include "CJSObject.h"
#include "CJSValue.h"

namespace cpp_javascriptcore
{

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
