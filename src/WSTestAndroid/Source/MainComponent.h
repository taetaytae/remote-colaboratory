/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "easywsclient.hpp"

using easywsclient::WebSocket;


//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent   : public AudioAppComponent, 
                      public Timer, 
                      public Button::Listener
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent();

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (Graphics& g) override;
    void resized() override;

    void buttonClicked(Button* button) override;

    void timerCallback() override;

    void wsCallback(const std::string & message);

private:
    //==============================================================================
    // Your private member variables go here...

    TextButton websockButton;
    Uuid programUniqueId;

    bool wsClientRunning; 

    WebSocket::pointer ws = NULL;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
