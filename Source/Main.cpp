#include "../JuceLibraryCode/JuceHeader.h"
#include "DroneGen.h"
#include "rang.h"
#define LIBCMDF_IMPL
#include "libcmdf.h" 
#include "doDrone.h"

#define PROG_INTRO "test - A simple test program for libcmdf.\n" \
	"You can use this as a reference on how to use the library!"
#define PRINTARGS_HELP "This is a very long help string for a command.\n" \
                       "As you can see, this is concatenated properly. It's pretty good!"
#define PRINTARGS_UNDOC_HELP "These are undocumented commands.\n" \
                       "Use at your own risk?"
static CMDF_RETURN do_hello(cmdf_arglist* arglist) {
	std::cout << "Hello, world!" << std::endl;

	return CMDF_OK;
}

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


int main(int argc, char* argv[])
{
	auto f = new File(File::getSpecialLocation(File::currentExecutableFile));
	auto s = f->getSize();
	auto ss = " welcome to sndedit "+String(s / 1024/1024)+"MB "+ String(s / 1024 % 1024)+"KB "+ String(s % 1024)+"B";
	auto q = static_cast<const char*>(ss.toUTF8());
	cmdf_init( " sndedit > ",  q, PROG_INTRO, PRINTARGS_UNDOC_HELP, '-', 1);

	cmdf_register_command(do_hello, "hello", "prints hello world");
	cmdf_register_command(do_printargs, "printargs", PRINTARGS_HELP);
	cmdf_register_command(doDrone, "drone", "plays a drone for specified number of seconds");
	cmdf_register_command(doDrone, "udrone", 0);
	


	juce::initialiseJuce_GUI();
	cmdf_commandloop();

	//DroneGen::playDrone(2);
	//std::cout << rang::bg::blue <<rang::fgB::magenta << "hello blue world"  << rang::bg::reset<<rang::fg::reset<<std::endl<<"goodbye"<<std::endl;
	juce::shutdownJuce_GUI();
	return 0;
}



//wm was here