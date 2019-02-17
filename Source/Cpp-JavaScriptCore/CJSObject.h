#pragma once

#include <JavaScriptCore/JavaScriptCore.h>

#include "CJSUtil.h"

namespace cpp_javascriptcore
{

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
        JSObjectSetProperty (
            context, object, getJSStringRefFromString (propertyName), value, kJSClassAttributeNone, NULL);
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

} // namespace cpp_javascriptcore
