#include <string>

#include "CJSConstructor.h"
#include "CJSObject.h"
#include "MagicConversion.h"
#include <JavaScriptCore/JavaScriptCore.h>

namespace cpp_javascriptcore
{

template <typename T> class CJSExportBuilder
{
public:
    CJSExportBuilder (std::string name_) : name (name_)
    {
    }

    ~CJSExportBuilder () = default;

    template <typename Fn> CJSExportBuilder& method (std::string name, Fn&& methodImpl)
    {
        auto jsMethod = makeMethodCallback<T> (methodImpl);
        prototypeMethods[name] = jsMethod;
        return *this;
    }

    CJSConstructor makeConstructor (JSContextRef context)
    {
        auto definition = kJSClassDefinitionEmpty;
        definition.className = name.c_str ();
        auto classRef = JSClassCreate (&definition);
        CJSConstructor::Constructor constructorFn =
            [=](JSContextRef localJsContext, JSObjectRef, size_t, const JSValueRef[], JSValueRef*) {
                auto* instance = new T ();
                return JSObjectMake (localJsContext, classRef, (void*)instance);
            };
        auto constructor = CJSConstructor (context, classRef, constructorFn);
        auto prototype = constructor.getPrototype ();

        for (auto pair : prototypeMethods)
        {
            auto methodName = pair.first;
            auto methodJsCallback = pair.second;
            auto jsFunction = CJSFunction (context, methodName, methodJsCallback).getValue ();
            prototype.setProperty (methodName, jsFunction);
        }

        return constructor;
    }

private:
    std::string name;
    std::unordered_map<std::string, CJSFunction::Callback> prototypeMethods;
};

template <typename T> CJSExportBuilder<T> class_ (std::string className)
{
    return CJSExportBuilder<T> (className);
}

} // namespace cpp_javascriptcore
