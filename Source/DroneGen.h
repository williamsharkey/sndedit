/*
==============================================================================

AudioGen.h
Created: 22 Oct 2019 2:41:56am
Author:  william

==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class DroneGen : public AudioSource
{

private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DroneGen)
		Random random;


public:
	AudioDeviceManager mDeviceManager;
	int mod = 1;
	int step = 0;
	DroneGen() {}
	~DroneGen() {}

	static void playDrone(int seconds) {
		DroneGen source;
		source.mDeviceManager.initialise(0, 2, 0, false);
		AudioSourcePlayer player;
		player.setSource(&source);
		source.mDeviceManager.addAudioCallback(&player);
		Thread::sleep(1000 * seconds);
		source.mDeviceManager.removeAudioCallback(&player);
		player.setSource(0);
		std::cout << "Hello from play drone" << std::endl;
	}

	void prepareToPlay(int samplesPerBlockExpected,
		double sampleRate) override {
	}

	void releaseResources() override {

	}


	void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override {
		auto device = mDeviceManager.getCurrentAudioDevice();

		auto activeOutputChannels = device->getActiveOutputChannels();

		auto maxOutputChannels = activeOutputChannels.getHighestBit() + 1;


		for (auto channel = 0; channel < maxOutputChannels; ++channel)
		{
			if ((!activeOutputChannels[channel]))
			{
				bufferToFill.buffer->clear(channel, bufferToFill.startSample, bufferToFill.numSamples);
			}
			else
			{

				auto* outBuffer = bufferToFill.buffer->getWritePointer(channel, bufferToFill.startSample);

				for (auto sample = 0; sample < bufferToFill.numSamples; ++sample) {
					outBuffer[sample] = (float)((step) * 2 / (float)mod) + 1;// random.nextFloat() * .1;
					step = step + 1;
					if (step%mod == 0) {
						mod = mod + 1;
						step = 0;
					}
				}
			}
		}
	}
};
