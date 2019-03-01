#include "JSC-JUCE.h"

void registerConsole (CJSContext& context)
{
    auto console = CJSObject (context.getContext ());

    CJSFunction::Callback logFn = [](JSContextRef lcontext,
                                     JSObjectRef,
                                     JSObjectRef,
                                     size_t numArguments,
                                     const JSValueRef arguments[],
                                     JSValueRef*) {
        for (int i = 0; i < numArguments; i++)
        {
            auto value = CJSValue (lcontext, arguments[i]);
            std::cout << value.get<std::string> ();
            if (i != numArguments - 1)
            {
                std::cout << ' ';
            }
        }
        return JSValueMakeNull (lcontext);
    };

    console.setProperty ("log", CJSFunction (context.getContext (), "log", logFn).getValue ());
    context.getGlobalObject ().setProperty ("console", console.getValue ());
}

void registerReactNativeJuce (CJSContext& context)
{
}

Either<CJSValue, std::string> evaluateFile (CJSContext& context, const std::string& fileName)
{
    File bundleFile = File (fileName);
    return context.evaluateScript (bundleFile.loadFileAsString ().toStdString ());
}
