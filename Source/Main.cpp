#include "../JuceLibraryCode/JuceHeader.h"
#include "DroneGen.h"
#define LIBCMDF_IMPL
#include "libcmdf.h" 
#include "doDrone.h"

#define PROG_INTRO "    sndedit\n" \
	"    command line sound editing."
#define PRINTARGS_HELP "This is a very long help string for a command.\n" \
                       "As you can see, this is concatenated properly. It's pretty good!"
#define PRINTARGS_UNDOC_HELP "These are undocumented commands.\n" \
                       "Use at your own risk?"

static CMDF_RETURN do_printargs(cmdf_arglist* arglist) {
	if (!arglist) {
		std::cout << "No arguments provided!" << std::endl;
		return CMDF_OK;
	}

	std::cout << "Total arguments = " << arglist->count << std::endl;
	for (size_t i = 0; i < arglist->count; i++)
		std::cout << "Argument " << i << ": \'" << arglist->args[i] << "\'" << std::endl;

	std::cout << std::endl;

	return CMDF_OK;
}

int selectedDevice = 0;
int selectedAudioDevice = 0;
//juce::MidiOutput device = NULL;

static CMDF_RETURN midiOut(cmdf_arglist* arglist) {
	if (!arglist || arglist->count < 2 ) {
		std::cout << " 2 arguments required, " << arglist->count << " given" << std::endl;
		return CMDF_OK;
	}

	auto note = atoi(arglist->args[0]);
	std::cout << "  note number   : " << note <<  std::endl;
	
	auto dur = atoi(arglist->args[1]);
	std::cout << "  duration in ms: " << dur << std::endl;
	for (size_t i = 2; i < arglist->count; i++)
		std::cout << "  extra argument: " << arglist->args[i] << std::endl;

	auto devices = juce::MidiOutput::getAvailableDevices();
	if (devices.size() == 0) {
		std::cout << "  no midi devices!" << std::endl;
		return CMDF_OK;
	
	}
	if (devices.size() <= selectedDevice) {
		std::cout << "  cannot send midi to selected device #"<< selectedDevice<<" because there are only "<<devices.size() <<" devices available" << std::endl;
		return CMDF_OK;
	}
	auto d = juce::MidiOutput::openDevice(devices[selectedDevice].identifier);
	auto mon = MidiMessage::noteOn(1, note,  (juce::uint8)100);
	//mon.setTimeStamp(1);

	//(*d).sendMessageNow(mon);
	//Thread::sleep(dur);
	auto moff = MidiMessage::noteOn(1, note,(juce::uint8)0);
	//moff.setTimeStamp(2);
	
	auto mb = new MidiBuffer();
	(*mb).addEvent(mon,44100*4);
	std::cout << ((int)(44.1*dur)) << std::endl;
	(*mb).addEvent(moff, 4*44100+((int)(44.1*dur)));

	//(*d).sendMessageNow(moff);
	(*d).startBackgroundThread();
	
	(*d).sendBlockOfMessagesNow(*mb);
	delete mb;
	return CMDF_OK;
}

static CMDF_RETURN notesOff(cmdf_arglist*) {
	
	auto devices = juce::MidiOutput::getAvailableDevices();
	if (devices.size() == 0) {
		std::cout << "  no midi devices!" << std::endl;
		return CMDF_OK;

	}
	if (devices.size() <= selectedDevice) {
		std::cout << "  cannot send midi to selected device #" << selectedDevice << " because there are only " << devices.size() << " devices available" << std::endl;
		return CMDF_OK;
	}
	auto d = juce::MidiOutput::openDevice(devices[selectedDevice].identifier);
	
	auto notesoff = MidiMessage::allSoundOff(1);
	notesoff.setTimeStamp(1);

	(*d).sendMessageNow(notesoff);
	
	return CMDF_OK;
}



static CMDF_RETURN selectDevice(cmdf_arglist* arglist) {
	if (!arglist || arglist->count != 1) {
        size_t x = (!arglist) ? 0 : arglist->count;
		std::cout << "  1 argument required to select a device, " << x << " given" << std::endl;
		return CMDF_OK;
	}

	selectedDevice = atoi(arglist->args[0]);

	auto devices = juce::MidiOutput::getAvailableDevices();
	
	int devicesCount = devices.size();
	
	
	
    std::cout << "  device cnt "<<devicesCount <<std::endl;
    
	
    if (devicesCount == 0) {
		std::cout << "  no midi devices!" << std::endl;
		return CMDF_OK;
	}
	
    if (devicesCount <= selectedDevice) {
		std::cout << "  selected midi device #"<<selectedDevice<< ",  however there are only "<<devicesCount << " available" << std::endl;
		
	}
	for (int i = 0; i < devicesCount; i++)

		std::cout << "  "<< ((i == selectedDevice) ? "[" : " ") << i << ((i == selectedDevice) ? "]" : " ") << " : " << devices[i].name << ((i == selectedDevice) ? " (selected)" : "") << std::endl; //devices[i].identifier
	return CMDF_OK;

}

static CMDF_RETURN midiDevices(cmdf_arglist* ) {
	
	auto devices = juce::MidiOutput::getAvailableDevices();
	if (devices.size() == 0) {
		std::cout << "No midi devices!" << std::endl;
		return CMDF_OK;
	}
	for (int i = 0; i < devices.size(); i++)
		std::cout << "  " << ((i == selectedDevice) ? "[" : " ") << i << ((i == selectedDevice) ? "]" : " ") << " : " << devices[i].name << ((i == selectedDevice) ? " (selected)" : "") << std::endl; //devices[i].identifier

	return CMDF_OK;
}

static CMDF_RETURN audioDevices(cmdf_arglist*) {
	AudioDeviceManager dm;

	auto devices = &dm.getAvailableDeviceTypes();
	if (devices->size() == 0) {
		std::cout << "No midi devices!" << std::endl;
		return CMDF_OK;
	}
	for (int i = 0; i < devices->size(); i++) {
		auto x = devices->getUnchecked(i);
		auto sel = (i == selectedAudioDevice);

		std::cout << "  " << ( sel ? "[" : " ") << i << (sel ? "]" : " ") << " : " << x->getTypeName() << (sel ? " (selected)" : "") << std::endl; 
	}
		

	return CMDF_OK;
}

int main(int, char* [])
{
	auto f = new File(File::getSpecialLocation(File::currentExecutableFile));
	auto s = f->getSize();
	auto ss = " welcome to sndedit "+String(s / 1024/1024)+"MB "+ String(s / 1024 % 1024)+"KB "+ String(s % 1024)+"B";
	auto q = static_cast<const char*>(ss.toUTF8());
	cmdf_init( " sndedit > ",  q, PROG_INTRO, PRINTARGS_UNDOC_HELP, '-', 1);


	cmdf_register_command(do_printargs, "printargs", PRINTARGS_HELP);
	cmdf_register_command(doDrone, "drone", "plays a drone for specified number of seconds");
	cmdf_register_command(doTone, "tone", "plays a tone for specified number of seconds at specified hz");
	cmdf_register_command(midiOut, "midi", "outputs a midi note to a device");
	cmdf_register_command(notesOff, "off", "turns all notes off");
	cmdf_register_command(midiDevices, "lm", "list midi devices");
	cmdf_register_command(audioDevices, "la", "list audio devices");
	cmdf_register_command(selectDevice, "select", "select midi device");


	juce::initialiseJuce_GUI();
	cmdf_commandloop();
	juce::shutdownJuce_GUI();
	return 0;
}



//wm was here
