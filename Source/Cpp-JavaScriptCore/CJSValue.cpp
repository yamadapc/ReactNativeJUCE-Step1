#include "CJSValue.h"

namespace cpp_javascriptcore
{

CJSValue::CJSValue (JSContextRef context_, JSValueRef value_) : context (context_), value (value_)
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

CJSValue::CJSType CJSValue::jsTypeToCJSType (JSType type)
{
    return static_cast<CJSValue::CJSType> (type);
}

JSValueRef CJSValue::getValue ()
{
    return value;
}

template <> std::string CJSValue::get ()
{
    auto jsStr = JSValueToStringCopy (context, value, nullptr);

    if (jsStr == nullptr)
    {
        return "";
    }

    std::string str;
    str.resize (JSStringGetLength (jsStr));
    auto* jsStrPtr = JSStringGetCharactersPtr (jsStr);

    for (int i = 0; i < str.size (); i++)
    {
        str[i] = jsStrPtr[i]; // NOLINT
    }

    return str;
}

template <> double CJSValue::get ()
{
    return JSValueToNumber (context, value, nullptr);
}

template <> bool CJSValue::get ()
{
    return JSValueToBoolean (context, value);
}

} // namespace cpp_javascriptcore
