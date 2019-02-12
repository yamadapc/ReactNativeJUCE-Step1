#include "Cpp-JavaScriptCore.h"

namespace cpp_javascriptcore
{
    JSStringRef getJSStringRefFromString (const std::string& str)
    {
        return JSStringCreateWithUTF8CString(str.c_str ());
    }

    CJSValue::CJSValue (JSContextRef context_, JSValueRef value_) : context (context_), value (value_)
    {
    }

    CJSValue::~CJSValue ()
    {
    }

    CJSValue::CJSType CJSValue::getType ()
    {
        return jsTypeToCJSType (JSValueGetType (context, value));
    }

    bool CJSValue::isUndefined ()
    {
        return JSValueIsUndefined (context, value);
    }

    bool CJSValue::isNull ()
    {
        return JSValueIsNull (context, value);
    }

    bool CJSValue::isBoolean ()
    {
        return JSValueIsBoolean (context, value);
    }

    bool CJSValue::isNumber ()
    {
        return JSValueIsNumber (context, value);
    }

    bool CJSValue::isString ()
    {
        return JSValueIsString (context, value);
    }

    bool CJSValue::isObject ()
    {
        return JSValueIsObject (context, value);
    }

    CJSValue::CJSType CJSValue::jsTypeToCJSType(JSType type)
    {
        return static_cast<CJSValue::CJSType> (type);
    }

    JSValueRef CJSValue::getValue ()
    {
        return value;
    }

    CJSContext::CJSContext ()
    {
        context = JSGlobalContextCreate(NULL);
    }

    CJSContext::~CJSContext()
    {
        JSGlobalContextRelease (context);
    }

    CJSValue CJSContext::evaluateScript (std::string script)
    {
        JSStringRef jsScript = getJSStringRefFromString (script);
        JSValueRef error;
        JSValueRef result = JSEvaluateScript (context, jsScript, NULL, NULL, 1, &error);

        if (result == NULL)
        {
            std::cout << "Error while evaluating" << std::endl;
            char* errorStr = new char[100];
            JSStringRef errorJsStr = JSValueToStringCopy (context, error, NULL);
            JSStringGetUTF8CString (errorJsStr, errorStr, 100);
            std::cout << errorStr << std::endl;
            delete[] errorStr;
        }

        return {context, result};
    }

    CJSObject CJSContext::getGlobalObject ()
    {
        return {context, JSContextGetGlobalObject (context)};
    }

    JSGlobalContextRef CJSContext::getContext ()
    {
        return context;
    }
}
