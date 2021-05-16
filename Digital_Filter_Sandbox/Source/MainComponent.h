#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  :  public juce::AudioAppComponent,
                        public juce::Button::Listener,
                        public juce::FileDragAndDropTarget
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;

private:
    void buttonClicked(juce::Button* button) override;
    
    bool isInterestedInFileDrag(const juce::StringArray& files) override;
    
    void filesDropped(const juce::StringArray& files, int x, int y) override;
    
    bool isFileLoaded() const;

    juce::ToggleButton applyFilterButton;
    std::atomic<bool> filterApplied;
    
    std::atomic<bool> playing;
    std::atomic<int> sampleNumber;
    
    juce::TextButton playButton;
    juce::TextButton stopButton;
    
    juce::File currentFile;
    
    juce::AudioFormatManager fmtMan;
    
    std::unique_ptr<juce::AudioFormatReader> fileReader;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
