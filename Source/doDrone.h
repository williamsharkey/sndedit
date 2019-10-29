#pragma once

static CMDF_RETURN doDrone(cmdf_arglist * arglist) {
	if (!arglist) {
		std::cout << "No arguments provided!" << std::endl;
		return CMDF_OK;
	}

	for (size_t i = 0; i < arglist->count; i++)
	{
		if (i == 0)
		{
			std::cout << "       seconds    : " << atoi(arglist->args[i]) << std::endl;
		}
		else
		{
			std::cout << "       unused arg : " << i + 1 << ": \'" << arglist->args[i] << "\'" << std::endl;
		}
	}

	const auto i = atoi(arglist->args[0]);
	DroneGen::playDrone(i);
	std::cout << std::endl;

	return CMDF_OK;
}


static CMDF_RETURN doTone(cmdf_arglist * arglist) {
	if (!arglist || arglist->count < 2) {
		std::cout << "Not enough arguments provided!" << std::endl;
		return CMDF_OK;
	}

	for (size_t i = 0; i < arglist->count; i++)
	{
		if (i == 0)
		{
			std::cout << "       seconds    : " << atoi(arglist->args[i]) << std::endl;
		} else if (i == 1)
		{
			std::cout << "       hz         : " << atoi(arglist->args[i]) << std::endl;
		}
		else
		{
			std::cout << "       unused arg : " << i + 1 << ": \'" << arglist->args[i] << "\'" << std::endl;
		}
	}

	const auto seconds = atoi(arglist->args[0]);
	const auto hz = atoi(arglist->args[1]);
	ToneGen::playTone(seconds,hz);
	std::cout << std::endl;
	return CMDF_OK;
}