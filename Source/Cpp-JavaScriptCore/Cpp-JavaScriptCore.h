#pragma once

#include <JavaScriptCore/JavaScriptCore.h>
#include <functional>
#include <iostream>
#include <neither/either.hpp>
#include <optional>
#include <string>

#include "CJSClass.h"
#include "CJSFunction.h"
#include "CJSObject.h"
#include "CJSValue.h"

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

    template <typename Fn> void registerFunction (std::string name, Fn callback)
    {
        auto global = getGlobalObject ();
        auto jsCallback = CJSFunction<Fn> (context, name, callback);
        global.setProperty (name, jsCallback.getValue ());
    }

private:
    JSGlobalContextRef context;
};

} // namespace cpp_javascriptcore
