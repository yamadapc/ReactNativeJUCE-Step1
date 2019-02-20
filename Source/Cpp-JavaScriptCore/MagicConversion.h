#pragma once

#include "CJSValue.h"
#include <JavaScriptCore/JavaScriptCore.h>
#include <vector>

namespace cpp_javascriptcore
{

template <typename Arg> CJSValue convertValue (JSContextRef context, Arg value)
{
    return CJSValue (context, value);
}

std::vector<JSValueRef> convertValues (JSContextRef context);

template <typename Arg, typename... Args>
std::vector<JSValueRef> convertValues (JSContextRef context, Arg value, Args... tail)
{
    auto ref = convertValue (context, value);
    auto rest = convertValues (context, tail...);
    rest.insert (rest.begin (), ref.getValue ());
    return std::move (rest);
}

} // namespace cpp_javascriptcore
