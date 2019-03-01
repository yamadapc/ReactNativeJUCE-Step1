#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"
#include "../Cpp-JavaScriptCore/Cpp-JavaScriptCore.h"

using namespace cpp_javascriptcore;

void registerConsole (CJSContext& context);

void registerReactNativeJuce (CJSContext& context);

Either<CJSValue, std::string> evaluateFile (CJSContext& context, const std::string& fileName);
