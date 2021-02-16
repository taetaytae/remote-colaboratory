/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
#include "StepEditor.h"
#include "easywsclient.hpp"

using easywsclient::WebSocket;

class MainComponent   : public AudioAppComponent,
                        public Timer,
                        private ChangeListener,
                        private Slider::Listener
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent();

    //==============================================================================
    void sliderValueChanged (Slider*) override;
    void sliderDragEnded (Slider*) override;

    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (Graphics& g) override;
    void resized() override;
    
    void timerCallback() override;
    void wsCallback(const std::string & message);

private:
    //==============================================================================
    te::Engine engine { ProjectInfo::projectName };
    te::Edit edit { engine, te::createEmptyEdit(), te::Edit::forEditing, nullptr, 0 };
    te::TransportControl& transport { edit.getTransport() };

    TextButton settingsButton { "Settings" }, playPauseButton { "Play" }, randomiseButton { "Randomise" }, clearButton { "Clear" };
    Slider tempoSlider;
    std::unique_ptr<StepEditor> stepEditor;

    //==============================================================================
    te::StepClip::Ptr createStepClip();
    Array<File> createSampleFiles();
    void createSamplerPlugin (Array<File> defaultSampleFiles);

    //==============================================================================
    te::StepClip::Ptr getClip();
    void updatePlayButtonText();
    void changeListenerCallback (ChangeBroadcaster*) override;
    
    //WebSocket Variables
    Uuid programUniqueId;
    bool wsClientRunning;
    
    void sendSequencerState();
    void sendGlobalTempo();

    WebSocket::pointer ws = NULL;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
