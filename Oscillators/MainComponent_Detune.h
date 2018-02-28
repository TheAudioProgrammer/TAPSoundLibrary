/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainContentComponent   : public AudioAppComponent
							   , Slider::Listener
{
public:
    //==============================================================================
    MainContentComponent()
    {
        setSize (800, 600);

		mixerSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
		mixerSlider.setRange(0, 1);
		mixerSlider.addListener(this);
		addAndMakeVisible(mixerSlider);
		mixerLabel.setText("Waves Mix (Weighted)", dontSendNotification);
		mixerLabel.attachToComponent(&mixerSlider, true);
		addAndMakeVisible(mixerLabel);

		ampSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
		ampSlider.setRange(0, 1);
		ampSlider.addListener(this);
		addAndMakeVisible(ampSlider);
		ampLabel.setText("Amplification (Volume)", dontSendNotification);
		ampLabel.attachToComponent(&ampSlider, true);
		addAndMakeVisible(ampLabel);

		freqSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
		freqSlider.setRange(0, (12 * octaves) - 1);
		freqSlider.addListener(this);
		addAndMakeVisible(freqSlider);
		freqLabel.setText("Frequency (Notes)", dontSendNotification);
		freqLabel.attachToComponent(&freqSlider, true);
		addAndMakeVisible(freqLabel);

		octavesSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
		octavesSlider.setRange(-3, 3);
		octavesSlider.addListener(this);
		addAndMakeVisible(octavesSlider);
		octaveLabel.setText("Octaves Increment", dontSendNotification);
		octaveLabel.attachToComponent(&octavesSlider, true);
		addAndMakeVisible(octaveLabel);

		noteSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
		noteSlider.setRange(-12, 12);
		noteSlider.addListener(this);
		addAndMakeVisible(noteSlider);
		noteLabel.setText("Semitones Increment", dontSendNotification);
		noteLabel.attachToComponent(&noteSlider, true);
		addAndMakeVisible(noteLabel);

		fineSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
		fineSlider.setRange(-1, 1);
		fineSlider.addListener(this);
		addAndMakeVisible(fineSlider);
		fineLabel.setText("Note Fine", dontSendNotification);
		fineLabel.attachToComponent(&fineSlider, true);
		addAndMakeVisible(fineLabel);

		detuneSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
		detuneSlider.setRange(0, 10);
		detuneSlider.addListener(this);
		addAndMakeVisible(detuneSlider);
		detuneLabel.setText("Detune Value", dontSendNotification);
		detuneLabel.attachToComponent(&detuneSlider, true);
		addAndMakeVisible(detuneLabel);

		voiceSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
		voiceSlider.setRange(0, maxVoices);
		voiceSlider.addListener(this);
		addAndMakeVisible(voiceSlider);
		voiceLabel.setText("Voices Played", dontSendNotification);
		voiceLabel.attachToComponent(&voiceSlider, true);
		addAndMakeVisible(voiceLabel);

		detuneSpreadSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
		detuneSpreadSlider.setRange(0, 48);
		detuneSpreadSlider.addListener(this);
		addAndMakeVisible(detuneSpreadSlider);
		detuneSpreadLabel.setText("Detune Spread (Semitones)", dontSendNotification);
		detuneSpreadLabel.attachToComponent(&detuneSpreadSlider, true);
		addAndMakeVisible(detuneSpreadLabel);

        // specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
    }

    ~MainContentComponent()
    {
        shutdownAudio();
    }

	void sliderValueChanged(Slider* slider) override {
		if (slider == &mixerSlider) {
			mixer = mixerSlider.getValue(); replace();
		}
		if (slider == &ampSlider) {
			amplitude = ampSlider.getValue();
		}
		if (slider == &freqSlider) {
			frequencyNote = round(freqSlider.getValue());
			freqSlider.setValue(frequencyNote);
			updateFrequency();
		}
		if (slider == &octavesSlider) {
			octaveV = round(octavesSlider.getValue());
			octavesSlider.setValue(octaveV);
			updateFrequency();
		}
		if (slider == &noteSlider) {
			noteV = round(noteSlider.getValue());
			noteSlider.setValue(noteV);
			updateFrequency();
		}
		if (slider == &fineSlider) {
			fineV = fineSlider.getValue();
			updateFrequency();
		}
		if (slider == &detuneSlider) {
			detune = detuneSlider.getValue(); updateFrequency();
		}
		if (slider == &voiceSlider) {
			voices = round(voiceSlider.getValue());
			voiceSlider.setValue(voices);
			updateFrequency();
		}
		if (slider == &detuneSpreadSlider) {
			detuneSpread = round(detuneSpreadSlider.getValue());
			detuneSpreadSlider.setValue(detuneSpread);
			updateFrequency();
		}
	}

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override
    {
        // This function will be called when the audio device is started, or when
        // its settings (i.e. sample rate, block size, etc) are changed.

        // You can use this function to initialise any resources you might need,
        // but be careful - it will be called on the audio thread, not the GUI thread.

        // For more details, see the help for AudioProcessor::prepareToPlay()

		currentSampleRate = sampleRate;
		frequency = 220;
		phase = 0;
		wavetableSize = 2048;
		phaseIncrement = frequency * wavetableSize / currentSampleRate;
		twoDoublePi = 2.0 * double_Pi;
		amplitude = 0.0;
		mixer = 0.0;
		detune = 0.0;
		detuneSpread = 0.0;
		frequencyNote = 0;
		octaveV = 0;
		fineV = 0;
		noteV = 0;

		for (int i = 0; i < maxVoices; i++) {
			freqV.insert(i, 0.0);
			phaseV.insert(i, 0.0);
			phaseIncV.insert(i, 0.0);
			outV.insert(i, 0.0);
		}


		//C0
		notesFrequency.insert(0, 16.351);
		//C#0
		notesFrequency.insert(1, 17.324);
		//D0
		notesFrequency.insert(2, 18.354);
		//D#0
		notesFrequency.insert(3, 19.445);
		//E0
		notesFrequency.insert(4, 20.601);
		//F0
		notesFrequency.insert(5, 21.827);
		//F#0
		notesFrequency.insert(6, 23.124);
		//G0
		notesFrequency.insert(7, 24.499);
		//G#0
		notesFrequency.insert(8, 25.956);
		//A0
		notesFrequency.insert(9, 27.50);
		//A#0
		notesFrequency.insert(10, 29.135);
		//B0
		notesFrequency.insert(11, 30.868);

		for (int i = 1; i < octaves; i++) {
			for (int j = 0; j < 12; j++) {
				int index = i * 12 + j;
				double nextNoteFrequency = notesFrequency.getUnchecked(index - 12) * 2;
				notesFrequency.insert(index, nextNoteFrequency);
			}
		}

		for (int i = 0; i < wavetableSize; i++) {
			double x = twoDoublePi * (i / wavetableSize);

			double sine = 0;
			double square = 0;
			double saw = 0;
			double strangeSine = 0;
			double saw2 = 0;

			for (int n = 1; n <= 200; n++) {
				double twoN1 = (2 * n - 1);

				double squareT = sin(twoN1 * x) / twoN1;
				square = square + (squareT * 1 / 0.926);

				double sawT = sin(n * x) / n;
				saw = saw + (sawT / 1.836);

				double strangeT = (sin(n * x) / n) * (sin(4 * n * x) / n);
				strangeSine = strangeSine + (strangeT / 1.094);

				double saw2T = sin((n * x) / 4) / n;
				saw2 = saw2 + (saw2T / 1.836);
			}
			sine = sin(x);

			double firstWave = saw;
			double secondWave = square;

			waveLeft.insert(i, firstWave);
			waveRight.insert(i, secondWave);

			double mixed = (1 - mixer) * waveLeft[i] + mixer * waveRight[i];
			waveOut.insert(i, mixed);
		}
    }

	void replace() {
		for (int i = 0; i < wavetableSize; i++) {
			double mixed = (1 - mixer) * waveLeft[i] + mixer * waveRight[i];
			waveOut.set(i, mixed);
		}
	}

	void updateFrequency() {
		frequency = notesFrequency.getUnchecked(frequencyNote);
		double change = 0.0;
		if (octaveV != 0) {
			change += (octaveV * 12.0);
		}
		if (noteV != 0) {
			change += noteV;
		}
		if (fineV != 0) {
			change += fineV;
		}

		if (change != 0) {
			frequency = frequency * pow(2.0, (change * 100 / 1200));
		}

		if (voices > 0) {
			for (int i = 0; i < voices; i++) {
				int vIdx = i + 1;
				int dIdx = vIdx - ((int)(voices + 0.5) / 2);
				double vFreq = frequency + pow((dIdx * ((detune * detuneSpread * 10) / 1200)), 2.0);
				freqV.set(i, vFreq);
				phaseIncV.set(i, vFreq * wavetableSize / currentSampleRate);
			}
		}
		
	}

    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override
    {
        // Your audio-processing code goes here!

        // For more details, see the help for AudioProcessor::getNextAudioBlock()

        // Right now we are not producing any data, in which case we need to clear the buffer
        // (to prevent the output of random noise)
        //bufferToFill.clearActiveBufferRegion();

		float* leftOut = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);
		float* rightOut = bufferToFill.buffer->getWritePointer(1, bufferToFill.startSample);

		for (int sample = 0; sample < bufferToFill.numSamples; sample++) {

			double finalOut = 0.0;

			for (int i = 0; i < voices; i++) {
				double out = waveOut[(int)phaseV.getUnchecked(i)] * amplitude;
				finalOut = finalOut + out;
			}
			
			finalOut = finalOut / voices;

			leftOut[sample] = rightOut[sample] = finalOut;

			
			for (int i = 0; i < voices; i++) {
				double phaseVNew = fmod((phaseV.getUnchecked(i) + phaseIncV.getUnchecked(i)), wavetableSize);
				phaseV.set(i, phaseVNew);
			}
		}
    }

    void releaseResources() override
    {
        // This will be called when the audio device stops, or when it is being
        // restarted due to a setting change.

        // For more details, see the help for AudioProcessor::releaseResources()
    }

    //==============================================================================
    void paint (Graphics& g) override
    {
        // (Our component is opaque, so we must completely fill the background with a solid colour)
        g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

        // You can add your drawing code here!
    }

    void resized() override
    {
        // This is called when the MainContentComponent is resized.
        // If you add any child components, this is where you should
        // update their positions.

		const int space = 200;
		int y = 20;
		mixerSlider.setBounds(space, y, getWidth() - space - 20, 20); y += 30;
		ampSlider.setBounds(space, y, getWidth() - space - 20, 20); y += 30;
		freqSlider.setBounds(space, y, getWidth() - space - 20, 20); y += 60;

		octavesSlider.setBounds(space, y, getWidth() - space - 20, 20); y += 30;
		noteSlider.setBounds(space, y, getWidth() - space - 20, 20); y += 30;
		fineSlider.setBounds(space, y, getWidth() - space - 20, 20); y += 60;

		detuneSlider.setBounds(space, y, getWidth() - space - 20, 20); y += 30;
		detuneSpreadSlider.setBounds(space, y, getWidth() - space - 20, 20); y += 30;
		voiceSlider.setBounds(space, y, getWidth() - space - 20, 20); y += 60;
    }


private:
    //==============================================================================

    // Your private member variables go here...

	Array<float> waveLeft;
	Array<float> waveRight;
	Array<float> waveOut;
	double wavetableSize;
	int frequencyNote;
	int octaves = 6;
	Array<float> notesFrequency;
	double frequency;
	double phase;
	double phaseIncrement;
	double twoDoublePi;
	double amplitude;
	double mixer;
	int changes = 0;
	bool up = true;
	double currentSampleRate;

	Slider mixerSlider;
	Label mixerLabel;
	Slider ampSlider;
	Label ampLabel;
	Slider freqSlider;
	Label freqLabel;

	Slider octavesSlider;
	Label octaveLabel;
	int octaveV;
	Slider noteSlider;
	Label noteLabel;
	int noteV;
	Slider fineSlider;
	Label fineLabel;
	double fineV;

	Slider detuneSlider;
	Label detuneLabel;
	double detune;
	Slider voiceSlider;
	Label voiceLabel;
	int voices;
	int maxVoices = 24;
	Slider detuneSpreadSlider;
	Label detuneSpreadLabel;
	int detuneSpread;

	Array<double> freqV;
	Array<double> phaseIncV;
	Array<double> phaseV;
	Array<double> outV;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


// (This function is called by the app startup code to create our main component)
Component* createMainContentComponent()     { return new MainContentComponent(); }
