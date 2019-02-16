#include "CJSValue.h"

namespace cpp_javascriptcore
{

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

CJSValue::CJSType CJSValue::jsTypeToCJSType (JSType type)
{
    return static_cast<CJSValue::CJSType> (type);
}

JSValueRef CJSValue::getValue ()
{
    return value;
}

}
