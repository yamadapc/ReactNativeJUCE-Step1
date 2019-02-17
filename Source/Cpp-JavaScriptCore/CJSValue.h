#pragma once

#include <JavaScriptCore/JavaScriptCore.h>
#include <neither/Either.hpp>
#include <string>

namespace cpp_javascriptcore
{

using namespace neither;

class CJSValue
{
public:
    enum class CJSType
    {
        Undefined,
        Null,
        Boolean,
        Number,
        String,
        Object,
    };

    CJSValue (JSContextRef context_, JSValueRef value_);
    ~CJSValue () = default;

    CJSType getType ();
    bool isUndefined ();
    bool isNull ();
    bool isBoolean ();
    bool isNumber ();
    bool isString ();
    bool isObject ();

    JSValueRef getValue ();

    template <typename T> Either<T, std::string> safeGet ();
    template <typename T> T get ();

private:
    static CJSType jsTypeToCJSType (JSType type);

    JSContextRef context;
    JSValueRef value;
};

} // namespace cpp_javascriptcore
