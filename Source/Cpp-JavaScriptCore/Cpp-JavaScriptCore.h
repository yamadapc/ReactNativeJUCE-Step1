#pragma once

#include <JavaScriptCore/JavaScriptCore.h>
#include <functional>
#include <iostream>
#include <neither/either.hpp>
#include <optional>
#include <string>

#include "CJSClass.h"
#include "CJSConstructor.h"
#include "CJSExportClass.h"
#include "CJSFunction.h"
#include "CJSObject.h"
#include "CJSValue.h"
#include "MagicConversion.h"

namespace cpp_javascriptcore
{

using namespace neither;

class CJSContext
{
public:
    CJSContext ();
    ~CJSContext ();

    Either<CJSValue, std::string> evaluateScript (const std::string& script);
    CJSObject getGlobalObject ();

    JSGlobalContextRef getContext ();

    CJSFunction registerFunction (std::string name, CJSFunction::Callback callback)
    {
        auto global = getGlobalObject ();
        auto jsCallback = CJSFunction (context, name, callback);
        global.setProperty (name, jsCallback.getValue ());
        return jsCallback;
    }

private:
    JSGlobalContextRef context;
};

} // namespace cpp_javascriptcore
