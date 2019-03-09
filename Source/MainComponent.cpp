/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent ()
{
    setSize (600, 400);

    registerConsole (context);

    evaluateFile (
            context,
            "/Users/yamadapc/Programming/github.com/beijaflor-io/ReactNativeJUCE-Step1/JavaScript/example/dist/main.js");

    context.evaluateScript(R"(
modules.register('Component', function register$Component() {
  return Component;
});
)");

/*    auto fn = [&](JSContextRef jsContext,
                  JSValueRef,
                  JSValueRef,
                  size_t numArguments,
                  const JSValueRef arguments[],
                  JSValueRef*) {
        assert (numArguments == 1);

        CJSValue jsStr = {jsContext, arguments[0]};

        assert (jsStr.isString ());

        auto str = jsStr.get<std::string> ();

        std::cout << "  JavaScript said:" << std::endl;
        std::cout << str << std::endl;

        message = str;
        javascriptSaidHello = true;

        repaint ();

        return JSValueMakeNull (jsContext);
    };
    auto callback = context.registerFunction ("sayHello", fn);

    context.evaluateScript ("sayHello('js'); console.log(120, 'something')");*/
}

MainComponent::~MainComponent ()
{
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel ().findColour (ResizableWindow::backgroundColourId));

    g.setFont (Font (16.0f));
    g.setColour (Colours::white);
    g.drawText ("Hello " + message + " from JavaScript!", getLocalBounds (), Justification::centred, true);
}

void MainComponent::resized ()
{
    // This is called when the MainComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
}
