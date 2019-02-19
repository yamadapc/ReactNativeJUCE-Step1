#include "JSC-JUCE.h"

JSObjectRef makeComponentClass (CJSContext& context)
{
    auto jsContext = context.getContext ();

    JSClassDefinition definition = kJSClassDefinitionEmpty;
    definition.className = "JUCE_Component";
    auto classRef = JSClassCreate (&definition);

    auto jsConstructor = JSObjectMakeConstructor (jsContext, classRef, nullptr);
    auto jsPrototype = JSObjectGetPrototype (jsContext, jsConstructor);

    auto prototype = CJSObject (jsContext, jsPrototype);
    auto addAndMakeVisible = CJSFunction (
        jsContext, [](JSContextRef, JSObjectRef, JSObjectRef thisValue, size_t, const JSValueRef[], JSValueRef*) {});

    prototype.setProperty ("addAndMakeVisible", addAndMakeVisible.getValue ());

    return jsConstructor;
}
