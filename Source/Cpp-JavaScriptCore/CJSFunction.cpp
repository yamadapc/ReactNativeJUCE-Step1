#include "CJSFunction.h"

namespace cpp_javascriptcore
{

template <>
JSValueRef callAsFunction<CbJSArgsReturningJSValue> (JSContextRef context,
                                                     JSObjectRef function,
                                                     JSObjectRef thisObject,
                                                     size_t numArguments,
                                                     const JSValueRef arguments[],
                                                     JSValueRef* error)
{
    void* data = JSObjectGetPrivate (function);
    auto* callable = static_cast<CbJSArgsReturningJSValue*> (data);
    return callable->operator() (thisObject, numArguments, arguments);
}

template <>
JSValueRef callAsFunction<CbJSArgs> (JSContextRef context,
                                     JSObjectRef function,
                                     JSObjectRef thisObject,
                                     size_t numArguments,
                                     const JSValueRef arguments[],
                                     JSValueRef* error)
{
    void* data = JSObjectGetPrivate (function);
    auto* callable = static_cast<CbJSArgs*> (data);
    callable->operator() (thisObject, numArguments, arguments);
    return JSValueMakeNull (context);
}

template <>
JSValueRef callAsFunction<CbJSArgsReturningJSValueWithContext> (JSContextRef context,
                                                                JSObjectRef function,
                                                                JSObjectRef thisObject,
                                                                size_t numArguments,
                                                                const JSValueRef arguments[],
                                                                JSValueRef* error)
{
    void* data = JSObjectGetPrivate (function);
    auto* callable = static_cast<CbJSArgsReturningJSValueWithContext*> (data);
    return callable->operator() (context, thisObject, numArguments, arguments);
}

template <>
JSValueRef callAsFunction<CbJSArgsWithContext> (JSContextRef context,
                                                JSObjectRef function,
                                                JSObjectRef thisObject,
                                                size_t numArguments,
                                                const JSValueRef arguments[],
                                                JSValueRef* error)
{
    void* data = JSObjectGetPrivate (function);
    auto* callable = static_cast<CbJSArgsWithContext*> (data);
    callable->operator() (context, thisObject, numArguments, arguments);
    return JSValueMakeNull (context);
}

template <>
JSValueRef callAsFunction<CbVoid> (JSContextRef context,
                                   JSObjectRef function,
                                   JSObjectRef thisObject,
                                   size_t numArguments,
                                   const JSValueRef arguments[],
                                   JSValueRef* error)
{
    void* data = JSObjectGetPrivate (function);
    auto* callable = static_cast<CbVoid*> (data);
    callable->operator() ();
    return JSValueMakeNull (context);
}

} // namespace cpp_javascriptcore
