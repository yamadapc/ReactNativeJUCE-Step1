#pragma once

#include <JavaScriptCore/JavaScriptCore.h>
#include <neither/Either.hpp>

#include "CJSClass.h"
#include "CJSUtil.h"
#include "CJSValue.h"
#include "MagicConversion.h"

namespace cpp_javascriptcore
{

using namespace neither;

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

    // TODO Safe variants of `setProperty...`, `getProperty...` methods

    void setProperty (const std::string& propertyName, JSValueRef value)
    {
        JSObjectSetProperty (
            context, object, getJSStringRefFromString (propertyName), value, kJSClassAttributeNone, nullptr);
    }

    CJSValue getProperty (const std::string& propertyName)
    {
        auto jsValue = JSObjectGetProperty (context, object, getJSStringRefFromString (propertyName), nullptr);
        assert (jsValue != nullptr);
        return {context, jsValue};
    }

    CJSValue operator[] (const std::string& key)
    {
        return getProperty (key);
    }

    CJSValue getPropertyAtIndex (unsigned propertyIndex)
    {
        auto jsValue = JSObjectGetPropertyAtIndex (context, object, propertyIndex, nullptr);
        assert (jsValue != nullptr);
        return {context, jsValue};
    }

    CJSValue operator[] (unsigned index)
    {
        return getPropertyAtIndex (index);
    }

    void* getPrivate ()
    {
        return JSObjectGetPrivate (object);
    }

    bool setPrivate (void* data)
    {
        return JSObjectSetPrivate (object, data);
    }

    bool isFunction ()
    {
        return JSObjectIsFunction (context, object);
    }

    Either<CJSValue, std::string>
    callAsFunction (JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[]);

    template <typename... Args> Either<CJSValue, std::string> call (Args... arguments)
    {
        auto jsArguments = convertValues (context, arguments...);
        return callAsFunction (nullptr, jsArguments.size (), jsArguments.data ());
    }

    Either<CJSValue, std::string>
    callMethod (const std::string& methodName, size_t argumentCount, const JSValueRef arguments[]);

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
