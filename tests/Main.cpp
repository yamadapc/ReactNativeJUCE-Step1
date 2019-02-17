#define CATCH_CONFIG_MAIN

#include "../Source/Cpp-JavaScriptCore/Cpp-JavaScriptCore.h"
#include "catch2/catch.hpp"
#include <string>

using namespace cpp_javascriptcore;

SCENARIO ("CJSContext")
{
    WHEN ("Its constructed/destructed")
    {
        CJSContext context;

        THEN ("Construction is fine")
        {
            REQUIRE (true);
        }

        THEN ("We can evaluate scripts")
        {
            auto result = context.evaluateScript ("10 + 2").left ().unsafeGet ();
            JSValueRef resultValue = result.getValue ();
            JSValueRef expectedValue = JSValueMakeNumber (context.getContext (), 12.0);

            REQUIRE (JSValueIsStrictEqual (context.getContext (), resultValue, expectedValue));
        }
    }
}

SCENARIO ("CJSValue")
{
    WHEN ("::get<std::string> ()")
    {
        JSGlobalContextRef context = JSGlobalContextCreate (nullptr);

        WHEN ("it is not a string")
        {
            JSValueRef jsValue = JSValueMakeNumber (context, 10.0);
            CJSValue value (context, jsValue);

            THEN ("it'll get recognized as a number")
            {
                REQUIRE (!value.isString ());
                REQUIRE (!value.isUndefined ());
                REQUIRE (!value.isNull ());
                REQUIRE (!value.isBoolean ());
                REQUIRE (!value.isObject ());
                REQUIRE (value.isNumber ());
            }

            THEN ("it'll convert into its string representation")
            {
                auto str = value.get<std::string> ();
                REQUIRE (str.size () == 2);
                REQUIRE (str == "10");
            }
        }

        WHEN ("it is a string")
        {
            std::string sourceStr = "hello world";
            JSStringRef jsStr = JSStringCreateWithUTF8CString (sourceStr.c_str ());
            JSValueRef jsValue = JSValueMakeString (context, jsStr);
            CJSValue value (context, jsValue);

            THEN ("it'll get recognized as a string")
            {
                REQUIRE (!value.isUndefined ());
                REQUIRE (!value.isNull ());
                REQUIRE (!value.isBoolean ());
                REQUIRE (!value.isObject ());
                REQUIRE (!value.isNumber ());
                REQUIRE (value.isString ());
            }

            THEN ("it'll convert back to its source string")
            {
                auto str = value.get<std::string> ();
                REQUIRE (str.size () == sourceStr.size ());
                REQUIRE (str == sourceStr);
            }
        }

        JSGlobalContextRelease (context);
    }

    WHEN ("::get<double> ()")
    {
        JSGlobalContextRef context = JSGlobalContextCreate (nullptr);

        WHEN ("it's not a number")
        {
            WHEN ("it may convert to a numbeer")
            {
                std::string sourceStr = "10.5";
                JSStringRef jsStr = JSStringCreateWithUTF8CString (sourceStr.c_str ());
                JSValueRef jsValue = JSValueMakeString (context, jsStr);
                CJSValue value (context, jsValue);

                THEN ("it'll convert")
                {
                    auto number = value.get<double> ();
                    REQUIRE (number == 10.5);
                }
            }

            WHEN ("it may not convert to a numbeer")
            {
                std::string sourceStr = "10-5";
                JSStringRef jsStr = JSStringCreateWithUTF8CString (sourceStr.c_str ());
                JSValueRef jsValue = JSValueMakeString (context, jsStr);
                CJSValue value (context, jsValue);

                THEN ("it'll give nan")
                {
                    auto number = value.get<double> ();
                    REQUIRE (isnan (number));
                }
            }
        }

        WHEN ("it's a number")
        {
            JSValueRef jsValue = JSValueMakeNumber (context, 10.5);
            CJSValue value (context, jsValue);

            THEN ("it'll convert")
            {
                auto number = value.get<double> ();
                REQUIRE (number == 10.5);
            }
        }

        JSGlobalContextRelease (context);
    }

    WHEN ("::get<bool> ()")
    {
        JSGlobalContextRef context = JSGlobalContextCreate (nullptr);

        WHEN ("it's false")
        {
            JSValueRef jsValue = JSValueMakeBoolean (context, false);
            CJSValue value (context, jsValue);

            THEN ("it'll convert")
            {
                auto number = value.get<bool> ();
                REQUIRE (number == false);
            }
        }

        WHEN ("it's true")
        {
            JSValueRef jsValue = JSValueMakeBoolean (context, true);
            CJSValue value (context, jsValue);

            THEN ("it'll convert")
            {
                auto number = value.get<bool> ();
                REQUIRE (number == true);
            }
        }

        JSGlobalContextRelease (context);
    }

    WHEN ("::get<CJSObject> ()")
    {
        CJSContext context;

        WHEN ("it's an object")
        {
            auto jsObj = JSObjectMake (context.getContext (), nullptr, nullptr);
            CJSObject obj (context.getContext (), jsObj);
            std::string sourceStr = "there";
            JSStringRef jsStr = JSStringCreateWithUTF8CString (sourceStr.c_str ());
            JSValueRef jsProperty = JSValueMakeString (context.getContext (), jsStr);
            obj.setProperty ("hello", jsProperty);
            CJSValue value (context.getContext (), obj.getValue ());

            THEN ("it'll convert")
            {
                auto object = value.get<CJSObject> ();
                REQUIRE (object.getProperty ("hello").get<std::string> () == "there");
            }
        }
    }
}
