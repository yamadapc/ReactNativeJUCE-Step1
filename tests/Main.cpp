#define CATCH_CONFIG_MAIN

#include <iostream>
#include <string>

#include "../Source/Cpp-JavaScriptCore/Cpp-JavaScriptCore.h"
#include "catch2/catch.hpp"

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

    WHEN ("::executeScript (script)")
    {
        CJSContext context;

        WHEN ("throws an exception")
        {
            THEN ("the result is a `Right` representing the error string")
            {
                auto result = context.evaluateScript ("throw new Error('error')");
                REQUIRE (result);
                REQUIRE (result.right ().unsafeGet () == "Error: error");
            }
        }

        WHEN ("returns a value")
        {
            THEN ("the result is a `Left` representing the return value")
            {
                auto result = context.evaluateScript ("10 + 2");
                REQUIRE (!result);
                REQUIRE (result.left ().unsafeGet ().get<double> () == 12.0);
            }
        }
    }

    WHEN ("::registerFunction (fn)")
    {
        THEN ("the function is callable from JavaScript")
        {
            CJSContext context;

            auto callback = context.registerFunction (
                "helloCpp",
                [](JSContextRef jsContext, JSObjectRef, JSObjectRef, size_t, const JSValueRef[], JSValueRef*) {
                    return CJSValue (jsContext, 10.0).getValue ();
                });

            auto result = context.evaluateScript ("helloCpp() + 4").left ().unsafeGet ();
            REQUIRE (result.get<double> () == 14.0);
        }

        THEN ("the function may call into the this object")
        {
            CJSContext context;

            auto callback = context.registerFunction ("helloCpp",
                                                      [](JSContextRef jsContext,
                                                         JSObjectRef,
                                                         JSObjectRef jsThisObject,
                                                         size_t,
                                                         const JSValueRef[],
                                                         JSValueRef*) {
                                                          auto thisObject = CJSObject (jsContext, jsThisObject);
                                                          auto value = thisObject.getProperty ("something");
                                                          return CJSValue (jsContext, value.get<double> ()).getValue ();
                                                      });

            auto result = context.evaluateScript ("helloCpp.call({something: 10})");
            result.rightMap ([](auto error) {
                std::cout << error << std::endl;
                return error;
            });
            REQUIRE (!result);
            REQUIRE (result.left ().unsafeGet ().get<double> () == 10.0);
        }
    }
}

SCENARIO ("CJSObject")
{
    CJSContext context;

    WHEN ("::callAsFunction (this, argCount, arguments)")
    {
        WHEN ("there's no context")
        {
            WHEN ("the function returns properly")
            {
                auto fn =
                    context.evaluateScript ("function hello() { return Math.pow(10, 2); }; hello").left ().unsafeGet ();
                auto val = context.evaluateScript ("hello()").left ().unsafeGet ();

                REQUIRE (val.get<double> () == 100);

                REQUIRE (fn.isObject ());
                REQUIRE (!fn.safeGet<CJSObject> ());

                THEN ("we get the return value")
                {
                    auto result = fn.get<CJSObject> ().callAsFunction (nullptr, 0, nullptr);
                    REQUIRE (!result);
                    REQUIRE (result.left ().unsafeGet ().get<double> () == 100);
                }
            }
        }
    }

    WHEN ("::call (arguments...)")
    {
        WHEN ("the function returns properly")
        {
            auto fn =
                context.evaluateScript ("function pow(n, m) { return Math.pow(n, m); }; pow").left ().unsafeGet ();
            auto fnObj = fn.get<CJSObject> ();

            REQUIRE (fn.isObject ());
            REQUIRE (!fn.safeGet<CJSObject> ());

            THEN ("we get the return value")
            {
                auto result = fnObj.call (10, 2);
                REQUIRE (!result);
                REQUIRE (result.left ().unsafeGet ().get<double> () == 100);
            }
        }
    }

    WHEN ("::callMethod (methodName, argCount, arguments)")
    {
        WHEN ("the function returns properly")
        {
            auto value = context.evaluateScript ("const something = {hello() { return 'world'; }}; something")
                             .left ()
                             .unsafeGet ();

            REQUIRE (value.isObject ());
            REQUIRE (!value.safeGet<CJSObject> ());

            auto obj = value.get<CJSObject> ();

            THEN ("we get the return value")
            {
                auto eitherResult = obj.callMethod ("hello", 0, nullptr);
                REQUIRE (!eitherResult);
                REQUIRE (eitherResult.left ().unsafeGet ().get<std::string> () == "world");
            }
        }

        WHEN ("the function errors")
        {
            auto value = context.evaluateScript ("const something = {hello() { throw new Error('error'); }}; something")
                             .left ()
                             .unsafeGet ();

            REQUIRE (value.isObject ());
            REQUIRE (!value.safeGet<CJSObject> ());

            auto obj = value.get<CJSObject> ();

            THEN ("we get an error")
            {
                auto eitherResult = obj.callMethod ("hello", 0, nullptr);
                REQUIRE (eitherResult);
                REQUIRE (eitherResult.right ().unsafeGet () == "Error: error");
            }
        }

        WHEN ("calling methods with arguments")
        {
            auto value = context.evaluateScript ("const something = {hello(n) { return n + 10; }}; something")
                             .left ()
                             .unsafeGet ();

            REQUIRE (value.isObject ());
            REQUIRE (!value.safeGet<CJSObject> ());

            auto obj = value.get<CJSObject> ();

            THEN ("the argument is correctly fowarded to JavaScript")
            {
                JSValueRef arguments[1];
                arguments[0] = JSValueMakeNumber (context.getContext (), 10);
                auto eitherResult = obj.callMethod ("hello", 1, arguments);
                REQUIRE (!eitherResult);
                REQUIRE (eitherResult.left ().unsafeGet ().get<double> () == 20);
            }
        }
    }
}

SCENARIO ("CJSValue")
{
    WHEN ("working with JavaScript arrays")
    {
        CJSContext context;
        JSContextRef jsContext = context.getContext ();

        THEN ("we can detect a value is an array")
        {
            auto value = context.evaluateScript ("const a = [1, 2, 3]; a").left ().unsafeGet ();
            REQUIRE (value.isArray ());
            REQUIRE (value.isObject ());
        }

        THEN ("we can get its elements")
        {
            auto value = context.evaluateScript ("const a = [1, 2, 3]; a").left ().unsafeGet ();
            REQUIRE (value.isArray ());
            REQUIRE (value.isObject ());
            auto arr = value.get<CJSObject> ();

            REQUIRE (arr.getProperty ("length").get<double> () == 3);
            REQUIRE (arr.getPropertyAtIndex (0).get<double> () == 1);
            REQUIRE (arr.getPropertyAtIndex (1).get<double> () == 2);
            REQUIRE (arr.getPropertyAtIndex (2).get<double> () == 3);
        }
    }

    WHEN ("checking for a value's type")
    {
        CJSContext context;

        WHEN ("value is a string")
        {
            THEN ("it'll return true when checking for strings")
            {
                auto value = context.evaluateScript ("'hello world'").left ().unsafeGet ();
                REQUIRE (value.isString ());
                REQUIRE (!value.isObject ());
            }
        }

        WHEN ("value is an object")
        {
            THEN ("it'll return true when checking for objects")
            {
                auto value =
                    context.evaluateScript ("const something = {hello: 'there'}; something").left ().unsafeGet ();
                REQUIRE (!value.isString ());
                REQUIRE (value.isObject ());
            }
        }

        WHEN ("value is a function")
        {
            THEN ("it'll return true when checking for objects")
            {
                auto value =
                    context.evaluateScript ("function something () { return 10 }; something").left ().unsafeGet ();
                REQUIRE (!value.isString ());
                REQUIRE (value.isObject ());
            }
        }
    }

    WHEN ("::safeGet<std::string> ()")
    {
        CJSContext context;

        WHEN ("it throws while converting")
        {
            auto eitherValue = context.evaluateScript (
                "const something = { toString: () => { throw new Error('conversion error') } }; something");

            eitherValue.rightMap ([](auto error) {
                std::cout << "Creating object failed: " << error << std::endl;
                return error;
            });

            REQUIRE (!eitherValue);
            auto value = eitherValue.left ().unsafeGet ();

            REQUIRE (value.isObject ());
            REQUIRE (value.get<CJSObject> ().getProperty ("toString").isObject ());
            REQUIRE (value.get<CJSObject> ().getProperty ("toString").get<CJSObject> ().isFunction ());


            THEN ("we'll get the error as a `Right`")
            {
                auto str = value.safeGet<std::string> ();

                str.rightMap ([](auto error) {
                    std::cout << "Conversion failed: " << error << std::endl;
                    return right (std::move (error));
                });

                str.leftMap ([](auto str) {
                    std::cout << "Conversion worked (it shouldn't): " << str << std::endl;
                    return left (std::move (str));
                });

                REQUIRE (str.right ().unsafeGet () == "Error: conversion error");
            }
        }
    }

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

    WHEN ("being constructed")
    {
        CJSContext context;

        WHEN ("constructed from a string")
        {
            THEN ("will be equal to manual construction through the C API")
            {
                auto value = CJSValue (context.getContext (), "Hello world");
                std::string sourceStr = "Hello world";
                JSStringRef jsStr = JSStringCreateWithUTF8CString (sourceStr.c_str ());
                JSValueRef expectedValue = JSValueMakeString (context.getContext (), jsStr);

                REQUIRE (value.get<std::string> () == "Hello world");

                {
                    CJSValue cexpectedValue = {context.getContext (), expectedValue};
                    REQUIRE (cexpectedValue.get<std::string> () == "Hello world");
                }

                REQUIRE (JSValueIsStrictEqual (context.getContext (), value.getValue (), expectedValue));
            }
        }

        WHEN ("constructed from a double")
        {
            THEN ("will be equal to manual construction through the C API")
            {
                auto value = CJSValue (context.getContext (), 10.5);
                double sourceVal = 10.5;
                JSValueRef expectedValue = JSValueMakeNumber (context.getContext (), sourceVal);

                REQUIRE (JSValueIsStrictEqual (context.getContext (), value.getValue (), expectedValue));
            }
        }

        WHEN ("constructed from a boolean")
        {
            THEN ("will be equal to manual construction through the C API")
            {
                auto value = CJSValue (context.getContext (), true);
                bool sourceVal = true;
                JSValueRef expectedValue = JSValueMakeBoolean (context.getContext (), sourceVal);

                REQUIRE (JSValueIsStrictEqual (context.getContext (), value.getValue (), expectedValue));
            }
        }
    }
}

SCENARIO ("Working with the C API to register classes")
{
    WHEN ("we try to define a constructor with some prototype methods")
    {
        CJSContext context;
        JSContextRef jsContext = context.getContext ();

        THEN ("things work out")
        {
            class Sample
            {
            public:
                Sample ()
                {
                }

                ~Sample () = default;

                std::string formatHello (std::string name)
                {
                    return "Hello there, " + name;
                }
            };

            auto definition = kJSClassDefinitionEmpty;
            definition.className = "Sample";
            static auto classRef = JSClassCreate (&definition);
            auto jsConstructor = JSObjectMakeConstructor (
                jsContext,
                classRef,
                [](JSContextRef localJsContext, JSObjectRef constructor, size_t, const JSValueRef[], JSValueRef*) {
                    auto* sample = new Sample ();
                    return JSObjectMake (localJsContext, classRef, (void*)sample);
                });
            auto jsPrototype = JSValueToObject (jsContext, JSObjectGetPrototype (jsContext, jsConstructor), nullptr);
            auto prototype = CJSObject (jsContext, jsPrototype);
            CJSFunction::Callback formatHello = [](JSContextRef localJsContext,
                                                   JSObjectRef,
                                                   JSObjectRef jsThisValue,
                                                   size_t argumentCount,
                                                   const JSValueRef arguments[],
                                                   JSValueRef*) {
                assert (argumentCount == 1);
                auto jsInputStr = arguments[0];
                auto inputStr = CJSValue (localJsContext, jsInputStr).get<std::string> ();
                auto thisValue = CJSObject (localJsContext, jsThisValue);
                void* privateData = thisValue.getPrivate ();
                auto* sample = (Sample*)privateData;
                return CJSValue (localJsContext, sample->formatHello (inputStr)).getValue ();
            };
            auto jsFormatHello = CJSFunction (jsContext, "formatHello", formatHello);
            prototype.setProperty ("formatHello", jsFormatHello.getValue ());
            context.getGlobalObject ().setProperty ("Sample", jsConstructor);

            auto eitherResult = context.evaluateScript (R"(
const sample = new Sample();
sample.formatHello('something');
)");
            REQUIRE (!eitherResult);
            auto result = eitherResult.left ().unsafeGet ();
            REQUIRE (result.get<std::string> () == "Hello there, something");
        }
    }
}

SCENARIO ("CJSConstructor")
{
    WHEN ("Trying to register classes with custom constructors")
    {
        CJSContext context;
        JSContextRef jsContext = context.getContext ();

        THEN ("CJSConstructor allows lambda constructor functions")
        {
            class Sample
            {
            public:
                Sample ()
                {
                }

                ~Sample () = default;

                std::string formatHello (std::string name)
                {
                    return "Hello there, " + name;
                }
            };

            auto definition = kJSClassDefinitionEmpty;
            definition.className = "Sample";
            auto classRef = JSClassCreate (&definition);
            CJSConstructor::Constructor constructorFn =
                [&](JSContextRef localJsContext, JSObjectRef, size_t, const JSValueRef[], JSValueRef*) {
                    auto* sample = new Sample ();
                    return JSObjectMake (localJsContext, classRef, (void*)sample);
                };
            auto constructor = CJSConstructor (jsContext, classRef, constructorFn);
            auto jsConstructor = constructor.getConstructor ();
            auto jsPrototype = JSValueToObject (jsContext, JSObjectGetPrototype (jsContext, jsConstructor), nullptr);
            auto prototype = CJSObject (jsContext, jsPrototype);
            CJSFunction::Callback formatHello = [](JSContextRef localJsContext,
                                                   JSObjectRef,
                                                   JSObjectRef jsThisValue,
                                                   size_t argumentCount,
                                                   const JSValueRef arguments[],
                                                   JSValueRef*) {
                assert (argumentCount == 1);
                auto jsInputStr = arguments[0];
                auto inputStr = CJSValue (localJsContext, jsInputStr).get<std::string> ();
                auto thisValue = CJSObject (localJsContext, jsThisValue);
                void* privateData = thisValue.getPrivate ();
                auto* sample = (Sample*)privateData;
                return CJSValue (localJsContext, sample->formatHello (inputStr)).getValue ();
            };
            auto jsFormatHello = CJSFunction (jsContext, "formatHello", formatHello);
            prototype.setProperty ("formatHello", jsFormatHello.getValue ());
            context.getGlobalObject ().setProperty ("Sample", jsConstructor);

            auto eitherResult = context.evaluateScript (R"(
const sample = new Sample();
sample.formatHello('something');
)");
            REQUIRE (!eitherResult);
            auto result = eitherResult.left ().unsafeGet ();
            REQUIRE (result.get<std::string> () == "Hello there, something");
        }
    }
}

SCENARIO ("convertValue(context, value)")
{
    CJSContext context;
    JSContextRef jsContext = context.getContext ();

    WHEN ("value is a char*")
    {
        auto value = convertValue (jsContext, "hello world");
        REQUIRE (value.get<std::string> () == "hello world");
    }

    WHEN ("value is a string")
    {
        std::string str = "hello world";
        auto value = convertValue (jsContext, str);
        REQUIRE (value.get<std::string> () == "hello world");
    }

    WHEN ("value is an int")
    {
        auto value = convertValue (jsContext, 10);
        REQUIRE (value.get<double> () == 10.0);
    }

    WHEN ("value is a float")
    {
        auto value = convertValue (jsContext, 10.0f);
        REQUIRE (value.get<double> () == 10.0);
    }

    WHEN ("value is a double")
    {
        auto value = convertValue (jsContext, 10.0);
        REQUIRE (value.get<double> () == 10.0);
    }

    WHEN ("value is a boolean")
    {
        auto value = convertValue (jsContext, true);
        REQUIRE (value.get<bool> () == true);
        auto value2 = convertValue (jsContext, false);
        REQUIRE (value2.get<bool> () == false);
    }
}

SCENARIO ("convertValues(context, ...values)")
{
    CJSContext context;
    JSContextRef jsContext = context.getContext ();

    WHEN ("values has no elements")
    {
        auto values = convertValues (jsContext);
        REQUIRE (values.size () == 0);
    }

    WHEN ("values has one char* element")
    {
        auto values = convertValues (jsContext, "hello world");
        REQUIRE (values.size () == 1);
        REQUIRE (CJSValue (jsContext, values[0]).get<std::string> () == "hello world");
    }

    WHEN ("values has one char* element and one double element")
    {
        auto values = convertValues (jsContext, "hello world", 10.0);
        REQUIRE (values.size () == 2);
        REQUIRE (CJSValue (jsContext, values[0]).get<std::string> () == "hello world");
        REQUIRE (CJSValue (jsContext, values[1]).get<double> () == 10.0);
    }
}
