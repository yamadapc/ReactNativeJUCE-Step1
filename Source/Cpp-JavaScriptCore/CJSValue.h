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
    CJSValue (JSContextRef context_, const char* str);
    CJSValue (JSContextRef context_, const std::string& str);
    CJSValue (JSContextRef context_, double number);
    CJSValue (JSContextRef context_, float number);
    CJSValue (JSContextRef context_, int number);
    CJSValue (JSContextRef context_, bool boolean);
    ~CJSValue () = default;

    CJSType getType ();
    bool isUndefined ();
    bool isNull ();
    bool isBoolean ();
    bool isNumber ();
    bool isString ();
    bool isObject ();
    bool isDate ();
    bool isArray ();

    JSValueRef getValue () const;

    template <typename T> Either<T, std::string> safeGet ();
    template <typename T> T get ();

    bool isStrictEqual (const CJSValue& rhs) const
    {
        return isStrictEqual (rhs.getValue ());
    }

    bool isStrictEqual (JSValueRef rhs) const
    {
        return JSValueIsStrictEqual (context, value, rhs);
    }

private:
    static CJSType jsTypeToCJSType (JSType type);

    JSContextRef context;
    JSValueRef value;
};

bool operator== (const CJSValue& lhs, const CJSValue& rhs);

} // namespace cpp_javascriptcore
