#include "Cpp-JavaScriptCore.h"

namespace cpp_javascriptcore
{

CJSContext::CJSContext ()
{
    context = JSGlobalContextCreate (NULL);
}

CJSContext::~CJSContext ()
{
    JSGlobalContextRelease (context);
}

CJSValue CJSContext::evaluateScript (std::string script)
{
    JSStringRef jsScript = getJSStringRefFromString (script);
    JSValueRef error;
    JSValueRef result = JSEvaluateScript (context, jsScript, NULL, NULL, 1, &error);

    if (result == NULL)
    {
        std::cout << "Error while evaluating" << std::endl;
        char* errorStr = new char[100];
        JSStringRef errorJsStr = JSValueToStringCopy (context, error, NULL);
        JSStringGetUTF8CString (errorJsStr, errorStr, 100);
        std::cout << errorStr << std::endl;
        delete[] errorStr;
    }

    return {context, result};
}

CJSObject CJSContext::getGlobalObject ()
{
    return {context, JSContextGetGlobalObject (context)};
}

JSGlobalContextRef CJSContext::getContext ()
{
    return context;
}

} // namespace cpp_javascriptcore
