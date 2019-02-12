#pragma once

#include <functional>
#include <string>
#include <optional>
#include <iostream>
#include <JavaScriptCore/JavaScriptCore.h>

namespace cpp_javascriptcore
{
    struct UndefinedLiteral {};
    struct NullLiteral {};

    JSStringRef getJSStringRefFromString (const std::string& str);

    class CJSClass
    {
    public:
        CJSClass (JSClassRef klass_) : klass (klass_) {}
        ~CJSClass () {}

        JSClassRef getClass ()
        {
            return klass;
        }

    private:
        JSClassRef klass;
    };

    class CJSObject
    {
    public:
        CJSObject (JSContextRef context_, JSObjectRef object_)
            : context (context_),
              object (object_)
        {
        }

        CJSObject (JSContextRef context, CJSClass klass, void* data)
            : CJSObject (context, JSObjectMake (context, klass.getClass (), data))
        {
        }

        CJSObject (JSContextRef context, std::string name, JSObjectCallAsFunctionCallback callback)
            : CJSObject (context, JSObjectMakeFunctionWithCallback (context, getJSStringRefFromString (name), callback))
        {
        }

        CJSObject (JSContextRef context_, std::string name, std::function<void()>& callback)
          : klass (JSClassCreate (makeFunctionDefinition ())),
            context (context_),
            object (JSObjectMake (context, *klass, NULL)),
            fn (callback)
        {
            std::cout << "Registering callback: " << &callback << std::endl;
            auto result = JSObjectSetPrivate (object, &callback);
            if (!result)
            {
                std::cout << "Failed" << std::endl;
            }
            std::cout << "Registered callback: " << JSObjectGetPrivate (object) << std::endl;
            std::cout << "Created function object: " << object << std::endl;
        }

        ~CJSObject () {}

        void setProperty (std::string propertyName, JSValueRef value)
        {
            JSObjectSetProperty (context, object, getJSStringRefFromString (propertyName), value, NULL, NULL);
        }

        JSValueRef getValue ()
        {
            return object;
        }

        JSObjectRef getObject ()
        {
            return object;
        }

    private:
        JSClassDefinition* makeFunctionDefinition ()
        {
            definition = kJSClassDefinitionEmpty;
            definition->callAsFunction = &callAsFunction;
            return &*definition;
        }

        static JSValueRef callAsFunction (JSContextRef context, JSObjectRef function, JSObjectRef, size_t, const JSValueRef[], JSValueRef*)
        {
            void* data = JSObjectGetPrivate (function);
            std::function<void()>* callable = (std::function<void()>*) data;
            callable->operator()();
            return JSValueMakeNull (context);
        }

        std::optional<JSClassDefinition> definition;
        std::optional<JSClassRef> klass;
        JSContextRef context;
        JSObjectRef object;
        std::optional<std::function<void ()>> fn;
    };

    // class (*JSObjectCallAsFunctionCallback) (JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception);

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
        ~CJSValue ();

        CJSType getType ();
        bool isUndefined ();
        bool isNull ();
        bool isBoolean ();
        bool isNumber ();
        bool isString ();
        bool isObject ();

        JSValueRef getValue ();

    private:
        static CJSType jsTypeToCJSType (JSType type);

        JSContextRef context;
        JSValueRef value;
    };

    class CJSContext
    {
    public:
        CJSContext ();
        ~CJSContext ();

        CJSValue evaluateScript (std::string script);
        CJSObject getGlobalObject ();

        JSGlobalContextRef getContext ();

        void registerFunction (std::string name, std::function<void()> callback)
        {
            auto global = getGlobalObject ();
            auto jsCallback = CJSObject (context, name, callback);
            global.setProperty (name, jsCallback.getValue ());
        }

    private:
        JSGlobalContextRef context;
    };
}
