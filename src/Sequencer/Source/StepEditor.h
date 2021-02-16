/*
  ==============================================================================

    StepEditor.h
    Created: 7 Mar 2020 12:55:32pm
    Author:  Mario A. Sánchez

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "Utilities.h"
#include "BinaryData.h"

//==============================================================================
/*
*/

//==============================================================================
namespace
{
    static void loadFileIntoSamplerChannel (te::StepClip& clip, int channelIndex, const File& f)
    {
        // Find SamplerPlugin for the Clip's Track
        if (auto sampler = clip.getTrack()->pluginList.findFirstPluginOfType<te::SamplerPlugin>())
        {
            // Update the Sound layer source
            sampler->setSoundMedia (channelIndex, f.getFullPathName());

            // Then update the channel name
            clip.getChannels()[channelIndex]->name = f.getFileNameWithoutExtension();
        }
        else
        {
            jassertfalse; // No SamplerPlugin added yet?
        }
    }
}

//==============================================================================
class StepEditor    : public Component,
                      private te::SelectableListener
{
public:
    StepEditor (te::StepClip& sc);
    ~StepEditor();
    
    te::StepClip::Pattern getPattern() const;

    void paint (Graphics&) override;
    void resized() override;
    
    struct ChannelConfig : public Component
    {
        ChannelConfig (StepEditor& se, int channel);
        
        void browseForAndLoadSample();
        void randomiseChannel();
        
        void resized() override;
        
        StepEditor& editor;
        const int channelIndex;
        ShapeButton loadButton { "L", Colours::white, Colours::white, Colours::white };
        ShapeButton randomiseButton { "R", Colours::white, Colours::white, Colours::white };
        Label nameLabel;
        Slider volumeSlider;
    };
    struct PatternEditor : public Component
    {
            PatternEditor (StepEditor& se);
            ~PatternEditor();
            
            void updatePaths();

            void paint (Graphics&) override;
            void resized() override;
            
            void mouseEnter (const MouseEvent& e) override;
            void mouseMove (const MouseEvent& e) override;
            void mouseDown (const MouseEvent& e) override;
            void mouseDrag (const MouseEvent& e) override;
            void mouseExit (const MouseEvent&) override;
        
            bool mouseIsDown;
        
            StepEditor& editor;

        private:
            void updateNoteXes();
            float getPlayheadX() const;
            int xToSequenceIndex (float x) const;
            Range<float> getSequenceIndexXRange (int index) const;
            
            bool setNoteUnderMouse (int newIndex, int newChannel);
            bool updateNoteUnderMouse (const MouseEvent& e);
            void setCellAtLastMousePosition (bool value);
            
            Array<float> noteXes;
            RectangleList<float> grid;
            Path activeCells, playingCells;
            Rectangle<float> playheadRect;
            int mouseOverCellIndex = -1, mouseOverChannel = -1;
            bool paintSolidCells = true;
            
    };
    
    PatternEditor patternEditor { *this };

private:
    te::StepClip& clip;
    te::TransportControl& transport;
    te::LambdaTimer timer;

    OwnedArray<ChannelConfig> channelConfigs;

    int yToChannel (float y) const;
    Range<float> getChannelYRange (int channelIndex) const;
    void selectableObjectChanged (te::Selectable*) override;
    void selectableObjectAboutToBeDeleted (te::Selectable*) override;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StepEditor)
};

//==============================================================================

