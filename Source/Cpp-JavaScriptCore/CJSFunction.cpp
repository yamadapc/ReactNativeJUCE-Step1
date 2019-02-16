#include "CJSFunction.h"

namespace cpp_javascriptcore
{

template <>
JSValueRef
callAsFunction<std::function<JSValueRef (JSObjectRef, size_t, const JSValueRef[])>> (JSContextRef context,
                                                                                     JSObjectRef function,
                                                                                     JSObjectRef thisObject,
                                                                                     size_t numArguments,
                                                                                     const JSValueRef arguments[],
                                                                                     JSValueRef* error)
{
    void* data = JSObjectGetPrivate (function);
    auto* callable = static_cast<std::function<JSValueRef (JSObjectRef, size_t, const JSValueRef[])>*> (data);
    return callable->operator() (thisObject, numArguments, arguments);
}

template <>
JSValueRef callAsFunction<std::function<void(JSObjectRef, size_t, const JSValueRef[])>> (JSContextRef context,
                                                                                         JSObjectRef function,
                                                                                         JSObjectRef thisObject,
                                                                                         size_t numArguments,
                                                                                         const JSValueRef arguments[],
                                                                                         JSValueRef* error)
{
    void* data = JSObjectGetPrivate (function);
    auto* callable = static_cast<std::function<void(JSObjectRef, size_t, const JSValueRef[])>*> (data);
    callable->operator() (thisObject, numArguments, arguments);
    return JSValueMakeNull (context);
}

template <>
JSValueRef callAsFunction<std::function<void(JSContextRef, JSObjectRef, size_t, const JSValueRef[])>> (
    JSContextRef context,
    JSObjectRef function,
    JSObjectRef thisObject,
    size_t numArguments,
    const JSValueRef arguments[],
    JSValueRef* error)
{
    void* data = JSObjectGetPrivate (function);
    auto* callable = static_cast<std::function<void(JSContextRef, JSObjectRef, size_t, const JSValueRef[])>*> (data);
    callable->operator() (context, thisObject, numArguments, arguments);
    return JSValueMakeNull (context);
}

template <>
JSValueRef callAsFunction<std::function<JSValueRef (JSContextRef, JSObjectRef, size_t, const JSValueRef[])>> (
    JSContextRef context,
    JSObjectRef function,
    JSObjectRef thisObject,
    size_t numArguments,
    const JSValueRef arguments[],
    JSValueRef* error)
{
    void* data = JSObjectGetPrivate (function);
    auto* callable =
        static_cast<std::function<JSValueRef (JSContextRef, JSObjectRef, size_t, const JSValueRef[])>*> (data);
    return callable->operator() (context, thisObject, numArguments, arguments);
}

template <>
JSValueRef callAsFunction<std::function<void()>> (JSContextRef context,
                                                  JSObjectRef function,
                                                  JSObjectRef thisObject,
                                                  size_t numArguments,
                                                  const JSValueRef arguments[],
                                                  JSValueRef* error)
{
    void* data = JSObjectGetPrivate (function);
    auto* callable = static_cast<std::function<void()>*> (data);
    callable->operator() ();
    return JSValueMakeNull (context);
}

} // namespace cpp_javascriptcore
