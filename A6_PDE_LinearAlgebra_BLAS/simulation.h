// simulation.h
#ifndef _SIMH_
#define _SIMH_

#include "wavetypes.h"

// The 'set_derived_simulation_parameters' function takes a Parameters
// struct that was read in from a parameters file, computes the other
// parameters needed for the simulation, and returns a new Parameters
// struct with the original values plus the derived ones.
Parameters set_derived_simulation_parameters(const Parameters& p);

// The 'create_simulation_system' allocates memory for the wave field
// at three time points and for the position array, and assigns these
// to the rarrays in the WaveState passed as arguments.
//
// Because these are rarrays, their memory will be
// automatically deallocated.
void create_simulation_system(const Parameters& param, WaveState& wave);

// The 'Linear_Algebra_Formulation' set the matrix and the paramter that we used for the time evolution in a linear algebra formulation, see the assignment
LinearAlgebra set_Linear_Algebra_Formulation(const Parameters& param, LinearAlgebra& matrix);

#endif
