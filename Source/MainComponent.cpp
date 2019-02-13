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
    auto fn = [&](JSValueRef, size_t numArguments, const JSValueRef arguments[]) {
        assert (numArguments == 1);

        auto str = arguments[0];

        std::cout << "  JavaScript said:" << std::endl;
        std::cout << str << std::endl;

        javascriptSaidHello = true;

        repaint ();
    };
    context.registerFunction ("sayHello", fn);
    File bundleFile = File (
        "/Users/yamadapc/Programming/github.com/beijaflor-io/ReactNativeJUCE-Step1/JavaScript/example/dist/main.js");
    context.evaluateScript (bundleFile.loadFileAsString ().toStdString ());
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
    if (javascriptSaidHello)
        g.drawText ("Hello World from JavaScript!", getLocalBounds (), Justification::centred, true);
}

void MainComponent::resized ()
{
    // This is called when the MainComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
}
