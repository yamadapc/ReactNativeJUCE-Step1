#include "Cpp-JavaScriptCore.h"

namespace cpp_javascriptcore
{

CJSContext::CJSContext ()
{
    context = JSGlobalContextCreate (nullptr);
}

CJSContext::~CJSContext ()
{
    JSGlobalContextRelease (context);
}

Either<CJSValue, std::string> CJSContext::evaluateScript (const std::string& script)
{
    JSStringRef jsScript = getJSStringRefFromString (script);
    JSValueRef error;
    JSValueRef result = JSEvaluateScript (context, jsScript, nullptr, nullptr, 1, &error);

    if (result == nullptr)
    {
        CJSValue cerror (context, error);
        return {{cerror.get<std::string> ()}};
    }

    return {{CJSValue (context, result)}};
}

CJSObject CJSContext::getGlobalObject ()
{
    return CJSObject (context, JSContextGetGlobalObject (context));
}

JSGlobalContextRef CJSContext::getContext ()
{
    return context;
}

} // namespace cpp_javascriptcore
