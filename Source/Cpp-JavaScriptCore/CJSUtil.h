#pragma once

#include <JavaScriptCore/JavaScriptCore.h>
#include <string>

namespace cpp_javascriptcore
{

JSStringRef getJSStringRefFromString (const std::string& str);

}
