#include <string>

#include "CJSConstructor.h"
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

    template <typename Fn> CJSExportBuilder& method (std::string name, Fn implementation);

    CJSConstructor makeConstructor (JSContextRef context)
    {
        auto definition = kJSClassDefinitionEmpty;
        definition.className = name.c_str ();
        auto classRef = JSClassCreate (&definition);
        CJSConstructor::Constructor constructorFn =
            [=](JSContextRef localJsContext, JSObjectRef, size_t, const JSValueRef[], JSValueRef*) {
                auto* instance = new T ();
                std::cout << "Building object" << std::endl;
                return JSObjectMake (localJsContext, classRef, (void*)instance);
            };
        auto constructor = CJSConstructor (context, classRef, constructorFn);
        return constructor;
    }

private:
    std::string name;
};

template <typename T> CJSExportBuilder<T> class_ (std::string className)
{
    return CJSExportBuilder<T> (className);
}

} // namespace cpp_javascriptcore
