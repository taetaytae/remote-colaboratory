/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"


void MainComponent::wsCallback(const std::string & message)
{
    var msg = JSON::parse(message);
    std::cout << "Message from: " << msg[0].toString() << std::endl;
    
    receivedMessage = msg[1].toString().toStdString();
    std::cout<<"Message: "<<receivedMessage<<std::endl;
    messageDisplay.setText("Received Message From: "+ msg[0].toString() + "\n" +
                           receivedMessage, NotificationType::dontSendNotification);
    
}

void MainComponent::timerCallback()
{
    // first time it is called, 
    // we'll start up the websocket client
    if (!wsClientRunning)
    {
        ws = WebSocket::from_url("ws://localhost:8080");
        jassert(ws);
        ws->send(programUniqueId.toDashedString().toStdString());
        wsClientRunning = true;
    }
    if(ws->getReadyState() != WebSocket::CLOSED) {
      ws->poll();
      ws->dispatch( [this](const std::string & message) {
            //Message/Package being received from server
            wsCallback(message);
            changeColourClicked();
        } 
      );
    }
}

void MainComponent::buttonClicked(Button* button)
{
    if (button == &websockButton)
    {
        var msg(programUniqueId.toDashedString().toStdString());
        var writtenMessage(messageEditor.getText());
        
        msg.append(writtenMessage);
        
        ws->send(JSON::toString(msg).toStdString());
        //std::cout<<msg[1].toString()<<std::endl;
    }
}

//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 600);

    addAndMakeVisible(websockButton);
    websockButton.setButtonText("Send message to server");
    websockButton.addListener(this);
    
    addAndMakeVisible(colourButton);
    colourButton.setButtonText("Change colour");
    colourButton.onClick = [this] { changeColourClicked(); };
    colourButton.addListener(this);
    
    addAndMakeVisible(messageDisplay);
    messageDisplay.setText("Received Message: ", NotificationType::dontSendNotification);
    
    addAndMakeVisible(messageEditor);
    messageEditor.setText("Message to send");
    
    receivedMessage = "";
    

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
    startTimer(100); // the number dictates the latency

}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
    delete ws;
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
    g.setColour(Colours::white);
    //g.drawText("Received Message:", 0, getHeight() / 1.7,  getWidth(), getHeight() / 8, Justification::centredBottom);
    //g.drawText(receivedMessage, 0, getHeight() / 1.6,  getWidth(), getHeight() / 8, Justification::centredBottom, true);
    
}

void MainComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
    websockButton.setBounds(0, 0, getWidth(), getHeight() / 4);
    colourButton.setBounds(0, getHeight() / 4, getWidth(), getHeight() / 4);
    messageEditor.setBounds(0, getHeight() / 2, getWidth(), getHeight() / 8);
    messageDisplay.setBounds(0, getHeight() / 1.6, getWidth(), getHeight() / 8);
}

void MainComponent::changeColourClicked()
{
    Colour randomColour (Random::getSystemRandom().nextInt());
    colourButton.setColour(colourButton.buttonColourId, randomColour);
}
