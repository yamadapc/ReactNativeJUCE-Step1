#include "CJSFunction.h"

namespace cpp_javascriptcore
{

CJSFunction::CJSFunction (JSContextRef context_, const std::string& name, Callback& callback)
    : context (context_),
      object (JSObjectMakeFunctionWithCallback (context, getJSStringRefFromString (name), &runCallback))
{
    callbackMap[object] = callback;
}

CJSFunction::~CJSFunction ()
{
    // callbackMap.erase (object);
}

JSValueRef CJSFunction::runCallback (JSContextRef context,
                                     JSObjectRef function,
                                     JSObjectRef thisObject,
                                     size_t numArguments,
                                     const JSValueRef arguments[],
                                     JSValueRef* error)
{
    auto maybeCallback = callbackMap.find (function);

    if (maybeCallback == callbackMap.end ())
    {
        return JSValueMakeNull (context);
    }

    return maybeCallback->second (context, function, thisObject, numArguments, arguments, error);
}

std::unordered_map<JSObjectRef, CJSFunction::Callback> CJSFunction::callbackMap;

} // namespace cpp_javascriptcore
