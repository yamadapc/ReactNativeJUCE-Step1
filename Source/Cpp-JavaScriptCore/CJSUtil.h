#pragma once

#include <JavaScriptCore/JavaScriptCore.h>
#include <string>

namespace cpp_javascriptcore
{

struct UndefinedLiteral
{
};

struct NullLiteral
{
};


JSStringRef getJSStringRefFromString (const std::string& str);

}
