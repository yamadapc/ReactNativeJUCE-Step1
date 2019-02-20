#pragma once

#include "CJSValue.h"
#include <JavaScriptCore/JavaScriptCore.h>

namespace cpp_javascriptcore
{

template <typename Arg> CJSValue convertValue (JSContextRef context, Arg value)
{
    return CJSValue (context, value);
}

} // namespace cpp_javascriptcore
