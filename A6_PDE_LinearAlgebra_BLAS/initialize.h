// initialize.h
#ifndef _INITH_
#define _INITH_

#include "wavetypes.h"

// The 'initialize' function fills the existing array x, rho, and
// rho_prev.
//
// The values of x will be position coordinates of the grid to use, as
// determined by the parameters passed through param, while rho and
// rho_prev are set to the same triangular wave form (see description
// of assignment 3).
void initialize(const Parameters& param, WaveState& wave);

#endif
