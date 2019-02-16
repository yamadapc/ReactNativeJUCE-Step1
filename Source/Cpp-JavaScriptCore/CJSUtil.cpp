#include "CJSUtil.h"

namespace cpp_javascriptcore
{

JSStringRef getJSStringRefFromString (const std::string& str)
{
    return JSStringCreateWithUTF8CString (str.c_str ());
}

} // namespace cpp_javascriptcore
