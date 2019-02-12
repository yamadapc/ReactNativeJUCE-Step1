/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    setSize (600, 400);
    auto fn = [&]() {
        std::cout << "  JavaScript said hello" << std::endl;
        javascriptSaidHello = true;
        repaint ();
    };
    context.registerFunction ("sayHello", fn);
    context.evaluateScript ("sayHello();");
}

MainComponent::~MainComponent()
{
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setFont (Font (16.0f));
    g.setColour (Colours::white);
    if (javascriptSaidHello)
        g.drawText ("Hello World from JavaScript!", getLocalBounds(), Justification::centred, true);
}

void MainComponent::resized()
{
    // This is called when the MainComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
}
