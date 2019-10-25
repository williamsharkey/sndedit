#include "../JuceLibraryCode/JuceHeader.h"
#include "DroneGen.h"
#include "rang.h"
#define LIBCMDF_IMPL
#include "libcmdf.h"

int main(int argc, char* argv[])
{
	cmdf_init("hello> ", "world", "yo", "hi", 3, 1);
	cmdf_commandloop();

	juce::initialiseJuce_GUI();
	DroneGen::playDrone(2);
	std::cout << rang::bg::blue <<rang::fgB::magenta << "hello blue world"  << rang::bg::reset<<rang::fg::reset<<std::endl<<"goodbye"<<std::endl;
	juce::shutdownJuce_GUI();
	return 0;
}

//wm was here