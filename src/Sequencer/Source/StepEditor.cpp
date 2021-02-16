/*
  ==============================================================================

    StepEditor.cpp
    Created: 7 Mar 2020 12:55:32pm
    Author:  Mario A. Sánchez

  ==============================================================================
*/

#include <JuceHeader.h>
#include "StepEditor.h"

//==============================================================================
//Struct ChannelConfig Implementation
StepEditor::ChannelConfig::ChannelConfig (StepEditor& se, int channel)
        : editor (se), channelIndex (channel)
{
    jassert (isPositiveAndBelow (channelIndex, editor.clip.getChannels().size()));
    nameLabel.getTextValue().referTo (editor.clip.getChannels()[channelIndex]->name.getPropertyAsValue());
    loadButton.onClick = [this] { browseForAndLoadSample(); };
    randomiseButton.onClick = [this] { randomiseChannel(); };

    volumeSlider.setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    volumeSlider.setTextBoxStyle (Slider::NoTextBox, 0, 0, false);
    volumeSlider.setRange (0, 127, 1);
    volumeSlider.getValueObject().referTo (editor.clip.getChannels()[channelIndex]->noteValue.getPropertyAsValue());
    volumeSlider.setDoubleClickReturnValue (true, te::StepClip::defaultNoteValue);

    loadButton.setShape (Icons::getFolderPath(), false, true, false);
    loadButton.setBorderSize (BorderSize<int> (2));
    randomiseButton.setShape (Icons::getDicePath(), false, true, false);

    Helpers::addAndMakeVisible (*this, { &nameLabel, &loadButton, &randomiseButton, &volumeSlider });
}

void StepEditor::ChannelConfig::browseForAndLoadSample()
{
    EngineHelpers::browseForAudioFile (editor.clip.edit.engine,
                                       [this] (const File& f) { loadFileIntoSamplerChannel (editor.clip, channelIndex, f); });
}

void StepEditor::ChannelConfig::randomiseChannel()
{
    editor.getPattern().randomiseChannel (channelIndex);
}

void StepEditor::ChannelConfig::resized()
{
    auto r = getLocalBounds();
    const int buttonH = roundToInt (r.getHeight() * 0.7);
    loadButton.setBounds (r.removeFromLeft (buttonH).reduced (2));
    randomiseButton.setBounds (r.removeFromLeft (buttonH).reduced (2));
    volumeSlider.setBounds (r.removeFromLeft (r.getHeight()));
    nameLabel.setBounds (r.reduced (2));
}

//==============================================================================
//==============================================================================
//Struct PatternEditor Implementation
StepEditor::PatternEditor::PatternEditor(StepEditor& se)
                        : editor (se)
{
    mouseIsDown = false;
}

StepEditor::PatternEditor::~PatternEditor()
{
}

void StepEditor::PatternEditor::updatePaths()
{
    playheadRect = {};
    grid.clear();
    activeCells.clear();
    playingCells.clear();

    const auto pattern (editor.clip.getPattern (0));
    const int numChans = editor.clip.getChannels().size();
    const float indent = 2.0f;

    const bool isPlaying = editor.transport.isPlaying();
    const float playheadX = getPlayheadX();

    for (int i = 0; i < numChans; ++i)
    {
        const BigInteger cache (pattern.getChannel (i));
        const Range<float> y (editor.getChannelYRange (i));
        int index = 0;
        float lastX = 0.0f;

        grid.addWithoutMerging ({ 0.0f, y.getStart() - 0.25f, (float) getWidth(), 0.5f });

        for (float x : noteXes)
        {
            if (cache[index])
            {
                Path& path = (isPlaying && playheadX >= lastX && playheadX < x)
                             ? playingCells : activeCells;

                const Rectangle<float> r (lastX, (float) y.getStart(), x - lastX, (float) y.getLength());
                path.addRoundedRectangle (r.reduced (jlimit (0.5f, indent, r.getWidth()  / 8.0f),
                                                     jlimit (0.5f, indent, r.getHeight() / 8.0f)), 2.0f);
            }

            lastX = x;
            ++index;
        }
    }

    // Add the vertical lines
    for (float x : noteXes)
        grid.addWithoutMerging ({ x - 0.25f, 0.0f, 0.5f, (float) getHeight() });

    // Add the missing right and bottom edges
    {
        auto r = getLocalBounds().toFloat();
        grid.addWithoutMerging (r.removeFromBottom (0.5f).translated (0.0f, -0.25f));
        grid.addWithoutMerging (r.removeFromLeft (0.5f).translated (-0.25f, 0.0f));
    }

    // Calculate playhead rect
    {
        auto r = getLocalBounds().toFloat();
        float lastX = 0.0f;

        for (float x : noteXes)
        {
            if (playheadX >= lastX && playheadX < x)
            {
                playheadRect = r.withX (lastX).withRight (x);
                break;
            }

            lastX = x;
        }
    }

    repaint();
}

void StepEditor::PatternEditor::paint (Graphics& g)
{
    g.setColour (Colours::white.withMultipliedAlpha (0.5f));
    g.fillRectList (grid);

    g.setColour (Colours::white.withMultipliedAlpha (0.7f));
    g.fillPath (activeCells);

    g.setColour (Colours::white);
    g.fillPath (playingCells);

    g.setColour (Colours::white.withMultipliedAlpha (0.5f));
    g.fillRect (playheadRect);

    const Range<float> x = getSequenceIndexXRange (mouseOverCellIndex);
    const Range<float> y = editor.getChannelYRange (mouseOverChannel);
    g.setColour (Colours::red);
    g.drawRect (Rectangle<float>::leftTopRightBottom (x.getStart(), y.getStart(), x.getEnd(), y.getEnd()), 2.0f);
}

void StepEditor::PatternEditor::resized()
{
    updateNoteXes();
    updatePaths();
}

//==============================================================================
void StepEditor::PatternEditor::mouseEnter(const MouseEvent &e)
{
    updateNoteUnderMouse (e);
}

void StepEditor::PatternEditor::mouseMove(const MouseEvent &e)
{
    updateNoteUnderMouse (e);
}

void StepEditor::PatternEditor::mouseDown(const MouseEvent &e)
{
    updateNoteUnderMouse (e);
    
    mouseIsDown = true;

    paintSolidCells = true;

    if (e.mods.isCtrlDown() || e.mods.isCommandDown()){     //Delete note on command/control click
        paintSolidCells = false;
    }
    else if (! e.mods.isShiftDown()){                       //Create note on a normal click
        paintSolidCells = ! editor.clip.getPattern (0).getNote (mouseOverChannel, mouseOverCellIndex);
    }
    
}

void StepEditor::PatternEditor::mouseDrag(const MouseEvent &e)
{
    updateNoteUnderMouse (e);
    setCellAtLastMousePosition (paintSolidCells);
}

void StepEditor::PatternEditor::mouseExit(const MouseEvent &)
{
    setNoteUnderMouse (-1, -1);
}

//==============================================================================

void StepEditor::PatternEditor::updateNoteXes()
{
    // We're going to do a quick bodge here just fit the notes to the current size
    noteXes.clearQuick();

    auto r = getLocalBounds().toFloat();
    const auto pattern (editor.clip.getPattern (0));
    const int numNotes = pattern.getNumNotes();
    const float w = r.getWidth() / std::max (1, numNotes);

    for (int i = 0; i < numNotes; ++i)
        noteXes.add ((i + 1) * w);
}

float StepEditor::PatternEditor::getPlayheadX() const
{
    auto clipRange = editor.clip.getEditTimeRange();

    if (clipRange.isEmpty())
        return 0.0f;

    const double position = editor.transport.position;
    const auto proportion = position / clipRange.getEnd();
    auto r = getLocalBounds().toFloat();

    return r.getWidth() * float (proportion);
}

int StepEditor::PatternEditor::xToSequenceIndex(float x) const
{
    if (x >= 0){
        for (int i = 0; i < noteXes.size(); ++i)
            if (x < noteXes.getUnchecked (i))
                return i;
    }

    return -1;
}

Range<float> StepEditor::PatternEditor::getSequenceIndexXRange (int index) const
{
    if (! isPositiveAndBelow (index, noteXes.size()) || noteXes.size() <= 2)
        return {};

    if (index == 0)
        return { 0.0f, noteXes.getUnchecked (index) };

    return { noteXes.getUnchecked (index - 1), noteXes.getUnchecked (index) };
}

bool StepEditor::PatternEditor::setNoteUnderMouse (int newIndex, int newChannel)
{
    if (newIndex != mouseOverCellIndex || newChannel != mouseOverChannel)
    {
        mouseOverCellIndex = newIndex;
        mouseOverChannel = newChannel;
        resized();
        repaint();
        return true;
    }

    return false;
}

bool StepEditor::PatternEditor::updateNoteUnderMouse (const MouseEvent& e)
{
    return setNoteUnderMouse (xToSequenceIndex ((float) e.x), editor.yToChannel ((float) e.y));
}

void StepEditor::PatternEditor::setCellAtLastMousePosition (bool value)
{
    editor.clip.getPattern (0).setNote (mouseOverChannel, mouseOverCellIndex, value);
}

//==============================================================================
//==============================================================================
StepEditor::StepEditor(te::StepClip& sc)
                : clip (sc), transport (sc.edit.getTransport())
{
    for (auto c : clip.getChannels())
        addAndMakeVisible (channelConfigs.add (new ChannelConfig (*this, c->getIndex())));

    addAndMakeVisible (patternEditor);

    timer.setCallback ([this] { patternEditor.updatePaths();});
    timer.startTimerHz (15);

    clip.addSelectableListener (this);

}

StepEditor::~StepEditor()
{
    clip.removeSelectableListener (this);
}

te::StepClip::Pattern StepEditor::getPattern() const
{
    return clip.getPattern (0);
}

void StepEditor::paint (Graphics& g)
{
}

void StepEditor::resized()
{
    auto r = getLocalBounds();
    auto configR = r.removeFromLeft (150);

    for (auto c : channelConfigs)
    {
        auto channelR = configR.toFloat();
        channelR.setVerticalRange (getChannelYRange (c->channelIndex));
        c->setBounds (channelR.getSmallestIntegerContainer());
    }

    patternEditor.setBounds (r);

}

int StepEditor::yToChannel (float y) const
{
    auto r = getLocalBounds().toFloat();
    const int numChans = clip.getChannels().size();

    if (r.isEmpty())
        return -1;

    return (int) std::floor (y / r.getHeight() * numChans);
}

Range<float> StepEditor::getChannelYRange (int channelIndex) const
{
    auto r = getLocalBounds().toFloat();
    const int numChans = clip.getChannels().size();

    if (numChans == 0)
        return {};

    const float h = r.getHeight() / numChans;

    return Range<float>::withStartAndLength (channelIndex * h, h);
}

void StepEditor::selectableObjectChanged (te::Selectable*)
{
    // This is our StepClip telling us that one of it's properties has changed
    patternEditor.updatePaths();
}

void StepEditor::selectableObjectAboutToBeDeleted (te::Selectable*)
{
    jassertfalse;
}
