#include "JSC-JUCE.h"

void registerConsole (CJSContext& context)
{
    auto console = context.makeObject ();

    console["log"] =
        [](JSContextRef lcontext, JSObjectRef, JSObjectRef, size_t numArguments, JSValueRef arguments[], JSValueRef*) {
            for (int i = 0; i < numArguments; i++)
            {
                auto value = CJSValue (lcontext, arguments[i]);
                std::cout << value.get<std::string> ();
                if (i != numArguments - 1)
                {
                    std::cout << ' ';
                }
            }
        };

    context["console"] = console;
}

void registerReactNativeJuce (CJSContext& context)
{
}
