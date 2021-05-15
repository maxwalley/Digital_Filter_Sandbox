#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()  : playButton("Play"), stopButton("Stop")
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (400, 300);

    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        setAudioChannels (2, 2);
    }
    
    applyFilterButton.addListener(this);
    addAndMakeVisible(applyFilterButton);
    
    playButton.addListener(this);
    addAndMakeVisible(playButton);
    
    stopButton.addListener(this);
    addAndMakeVisible(stopButton);
    
    filterApplied = false;
    playing = false;
    
    fmtMan.registerBasicFormats();
}

MainComponent::~MainComponent()
{
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    bufferToFill.clearActiveBufferRegion();
}

void MainComponent::releaseResources()
{
    
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    
    g.setColour(juce::Colours::white);
    g.setFont(juce::Font(20));
    g.drawText("Toggle Filter", 10, 10, 200, 20, juce::Justification::centred);
}

void MainComponent::resized()
{
    applyFilterButton.setBounds(95, 30, 30, 30);
    
    playButton.setBounds(getWidth() - 210, 10, 200, 20);
    stopButton.setBounds(getWidth() - 210, 40, 200, 20);
}

void MainComponent::buttonClicked(juce::Button* button)
{
    if(button == &applyFilterButton)
    {
        filterApplied = button->getToggleState();
    }
    else if(button == &playButton)
    {
        if(!playing && isFileLoaded())
        {
            //play file
        }
    }
    else if(button == &stopButton)
    {
        if(playing)
        {
            //stop file
        }
    }
}

bool MainComponent::isInterestedInFileDrag(const juce::StringArray& files)
{
    return files.size() == 1;
}

void MainComponent::filesDropped(const juce::StringArray& files, int x, int y)
{
    assert(files.size() == 1);
    
    juce::File newFile(files[0]);
    
    if(newFile.existsAsFile())
    {
        std::unique_ptr<juce::AudioFormatReader> potentialReader(fmtMan.createReaderFor(newFile));
        
        if(potentialReader)
        {
            fileReader.reset(potentialReader.release());
            return;
        }
    }
    
    fileReader.release();
}

bool MainComponent::isFileLoaded() const
{
    return fileReader != nullptr;
}
