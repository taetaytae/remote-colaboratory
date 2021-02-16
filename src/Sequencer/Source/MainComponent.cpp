/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    transport.addChangeListener (this);

    createStepClip();
    createSamplerPlugin (createSampleFiles());

    stepEditor = std::make_unique<StepEditor> (*getClip());
    Helpers::addAndMakeVisible (*this, { &settingsButton, &playPauseButton, &randomiseButton,
                                         &clearButton, &tempoSlider, stepEditor.get() });

    updatePlayButtonText();

    settingsButton.onClick  = [this] { EngineHelpers::showAudioDeviceSettings (engine); };
    playPauseButton.onClick = [this] { EngineHelpers::togglePlay (edit); };
    randomiseButton.onClick = [this] { getClip()->getPattern (0).randomiseSteps();
                                       sendSequencerState(); };
    
    clearButton.onClick     = [this] { getClip()->getPattern (0).clear();
                                       sendSequencerState(); };

    tempoSlider.setRange (30.0, 220.0, 0.1);
    tempoSlider.setValue (edit.tempoSequence.getTempos()[0]->getBpm(), dontSendNotification);
    tempoSlider.addListener (this);

    setSize (600, 400);

    // Some platforms require permissions to open input channels so request that here
    if (RuntimePermissions::isRequired (RuntimePermissions::recordAudio)
        && ! RuntimePermissions::isGranted (RuntimePermissions::recordAudio))
    {
        RuntimePermissions::request (RuntimePermissions::recordAudio,
                                     [&] (bool granted) { if (granted)  setAudioChannels (2, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
    }
    
    wsClientRunning = false;
    startTimer(60);
}

MainComponent::~MainComponent()
{
    // Clean up our temporary sample files and projects
    engine.getTemporaryFileManager().getTempDirectory().deleteRecursively();
    delete ws;
}

void MainComponent::timerCallback()
{
    //Set up websocket client connection if not setup
    if (!wsClientRunning)
      {
          ws = WebSocket::from_url("ws://localhost:8080");
          //ws = WebSocket::from_url("ws://172.16.11.47:8080");
          jassert(ws);
          
          var idPacket("id");
          var id(programUniqueId.toDashedString().toStdString());
          idPacket.append(id);
          
          var msg(idPacket);
          
          //ws->send(programUniqueId.toDashedString().toStdString());
          ws->send(JSON::toString(msg).toStdString());
          wsClientRunning = true;
      }
    
    //Receive incoming messages
      if(ws->getReadyState() != WebSocket::CLOSED) {
        ws->poll();
        ws->dispatch( [this](const std::string & message) {
              //Message/Package being received from server
              wsCallback(message);
          }
        );
      }
    
    if(wsClientRunning
       && stepEditor->patternEditor.mouseIsDown)
    {
        
        sendSequencerState();
        
        //Switch to false once message is sent once after one mouse press
        stepEditor->patternEditor.mouseIsDown = false;
    }
}

void MainComponent::wsCallback(const std::string & message)
{
    var msg = JSON::parse(message);
    
    if(msg[0] == "id"){
        std::cout << "Message from: " << msg[1].toString() << std::endl;
    }
    
    //Received Messages are going to have ID as first value and then each of the channel patterns
    std::cout << "Message type: " << msg[2][0].toString() << std::endl;
    if(msg[2][0] == "channeldata"){
        int numOfChannels = 8;
        
        for(int i = 0; i < numOfChannels; i++)
        {
            juce::BigInteger receivedChannel((int)msg[2][i+1]);
            stepEditor->patternEditor.editor.getPattern().setChannel(i, receivedChannel);
        }
    }
    
}

void MainComponent::sendSequencerState()
{
    //Store each Juce::BigInteger pattern sequence inside the var message
    
    var idPacket("id");
    var id(programUniqueId.toDashedString().toStdString());
    idPacket.append(id);
    
    var msg(idPacket);
    
    var sequencePacket("channeldata");
    var channelOne(stepEditor->patternEditor.editor.getPattern().getChannel(0).toInteger());
    var channelTwo(stepEditor->patternEditor.editor.getPattern().getChannel(1).toInteger());
    var channelThree(stepEditor->patternEditor.editor.getPattern().getChannel(2).toInteger());
    var channelFour(stepEditor->patternEditor.editor.getPattern().getChannel(3).toInteger());
    var channelFive(stepEditor->patternEditor.editor.getPattern().getChannel(4).toInteger());
    var channelSix(stepEditor->patternEditor.editor.getPattern().getChannel(5).toInteger());
    var channelSeven(stepEditor->patternEditor.editor.getPattern().getChannel(6).toInteger());
    var channelEight(stepEditor->patternEditor.editor.getPattern().getChannel(7).toInteger());
    
    sequencePacket.append(channelOne);
    sequencePacket.append(channelTwo);
    sequencePacket.append(channelThree);
    sequencePacket.append(channelFour);
    sequencePacket.append(channelFive);
    sequencePacket.append(channelSix);
    sequencePacket.append(channelSeven);
    sequencePacket.append(channelEight);
    
    msg.append(sequencePacket);

    ws->send(JSON::toString(msg).toStdString());
}

void MainComponent::sendGlobalTempo()
{
    var idPacket("id");
    var id(programUniqueId.toDashedString().toStdString());
    idPacket.append(id);
    
    var msg(idPacket);
    
    var transportPacket("globaltempo");
}
//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    // This function will be called when the audio device is started, or when
    // its settings (i.e. sample rate, block size, etc) are changed.

    // You can use this function to initialise any resources you might need,
    // but be careful - it will be called on the audio thread, not the GUI thread.

    // For more details, see the help for AudioProcessor::prepareToPlay()
}

void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    // Your audio-processing code goes here!

    // For more details, see the help for AudioProcessor::getNextAudioBlock()

    // Right now we are not producing any data, in which case we need to clear the buffer
    // (to prevent the output of random noise)
    bufferToFill.clearActiveBufferRegion();
}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    // You can add your drawing code here!
}

void MainComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
    auto r = getLocalBounds();

    {
        auto topR = r.removeFromTop (30);
        settingsButton.setBounds (topR.removeFromLeft (topR.getWidth() / 3).reduced (2));
        playPauseButton.setBounds (topR.removeFromLeft (topR.getWidth() / 2).reduced (2));
        randomiseButton.setBounds (topR.removeFromLeft (topR.getWidth() / 2).reduced (2));
        clearButton.setBounds (topR.reduced (2));
    }

    {
        auto bottomR = r.removeFromBottom (60);
        tempoSlider.setBounds (bottomR.reduced (2));
    }

    stepEditor->setBounds (r);
}

void MainComponent::sliderValueChanged (Slider*)
{
    if (! ModifierKeys::getCurrentModifiers().isAnyMouseButtonDown())
        edit.tempoSequence.getTempos()[0]->setBpm (tempoSlider.getValue());
}

void MainComponent::sliderDragEnded(Slider *)
{
    edit.tempoSequence.getTempos()[0]->setBpm (tempoSlider.getValue());
}

//==============================================================================
te::StepClip::Ptr MainComponent::createStepClip()
{
    if (auto track = EngineHelpers::getOrInsertAudioTrackAt (edit, 0))
    {
        // Find length of 1 bar
        const te::EditTimeRange editTimeRange (0, edit.tempoSequence.barsBeatsToTime ({ 1, 0.0 }));
        track->insertNewClip (te::TrackItem::Type::step, "Step Clip", editTimeRange, nullptr);

        if (auto stepClip = getClip())
            return EngineHelpers::loopAroundClip (*stepClip);
    }

    return {};
}

Array<File> MainComponent::createSampleFiles()
{
    Array<File> files;
    const auto destDir = edit.getTempDirectory (true);
    jassert (destDir != File());

    using namespace DemoBinaryData;

    for (int i = 0; i < namedResourceListSize; ++i)
    {
        const auto f = destDir.getChildFile (originalFilenames[i]);

        int dataSizeInBytes = 0;
        const char* data = getNamedResource (namedResourceList[i], dataSizeInBytes);
        jassert (data != nullptr);
        f.replaceWithData (data, (size_t) dataSizeInBytes);
        files.add (f);
    }

    return files;
}

void MainComponent::createSamplerPlugin(Array<File> defaultSampleFiles)
{
    if (auto stepClip = getClip())
    {
        if (auto sampler = dynamic_cast<te::SamplerPlugin*> (edit.getPluginCache().createNewPlugin (te::SamplerPlugin::xmlTypeName, {}).get()))
        {
            stepClip->getTrack()->pluginList.insertPlugin (*sampler, 0, nullptr);

            int channelCount = 0;

            for (auto channel : stepClip->getChannels())
            {
                const auto error = sampler->addSound (defaultSampleFiles[channelCount++].getFullPathName(), channel->name.get(), 0.0, 0.0, 1.0f);
                sampler->setSoundParams (sampler->getNumSounds() - 1, channel->noteNumber, channel->noteNumber, channel->noteNumber);
                jassert (error.isEmpty());

                for (auto& pattern : stepClip->getPatterns())
                    pattern.randomiseChannel (channel->getIndex());
            }
        }
    }
    else
    {
        jassertfalse; // StepClip not been created yet?
    }
}

te::StepClip::Ptr MainComponent::getClip()
{
    if (auto track = EngineHelpers::getOrInsertAudioTrackAt (edit, 0))
        if (auto clip = dynamic_cast<te::StepClip*> (track->getClips()[0]))
            return *clip;
    return {};
}

void MainComponent::updatePlayButtonText()
{
    playPauseButton.setButtonText (transport.isPlaying() ? "Pause" : "Play");
}

void MainComponent::changeListenerCallback(ChangeBroadcaster *)
{
    updatePlayButtonText();
}
