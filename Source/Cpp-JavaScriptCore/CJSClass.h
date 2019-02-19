#pragma once

#include <JavaScriptCore/JavaScriptCore.h>
#include <functional>
#include <unordered_map>

namespace cpp_javascriptcore
{

class CJSClass
{
public:
    CJSClass (JSClassRef klass_) : klass (klass_)
    {
    }

    ~CJSClass ()
    {
    }

    JSClassRef getClass ()
    {
        return klass;
    }

private:
    JSClassRef klass;
};

} // namespace cpp_javascriptcore
