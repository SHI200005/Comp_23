// parameters.h
#ifndef _PARAMETERSH_
#define _PARAMETERSH_

#include "wavetypes.h"

// The function 'readParametersFromFile' reads the non-derived
// parameters from the file called filename, and puts them into the
// param structure.
//
// If there is an error in reading the file or in the values it sets,
// the function will return a non-zero number.
int readParametersFromFile(const std::string& filename, Parameters& param);

#endif
