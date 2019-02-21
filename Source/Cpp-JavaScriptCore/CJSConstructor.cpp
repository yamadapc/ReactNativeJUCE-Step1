#include "CJSConstructor.h"

namespace cpp_javascriptcore
{

CJSConstructor::CJSConstructor (JSContextRef context_, JSClassRef klass_, Constructor& constructor)
    : context (context_), klass (klass_), jsConstructor (JSObjectMakeConstructor (context, klass, &runConstructor))
{
    constructorMap[jsConstructor] = constructor;
}

CJSConstructor::~CJSConstructor ()
{
    // constructorMap.erase (jsConstructor);
}

JSObjectRef CJSConstructor::runConstructor (JSContextRef context,
                                            JSObjectRef constructor,
                                            size_t numArguments,
                                            const JSValueRef arguments[],
                                            JSValueRef* error)
{
    auto maybeConstructor = constructorMap.find (constructor);

    if (maybeConstructor == constructorMap.end ())
    {
        assert (false);
        return nullptr;
    }

    return maybeConstructor->second (context, constructor, numArguments, arguments, error);
}

std::unordered_map<JSObjectRef, CJSConstructor::Constructor> CJSConstructor::constructorMap;

} // namespace cpp_javascriptcore
