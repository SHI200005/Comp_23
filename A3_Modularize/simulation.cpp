// simulation.cpp
#include "simulation.h"
#include <tuple>
#include <memory>

// See simulation.h for how to use the functions in this module

Parameters set_derived_simulation_parameters(const Parameters& p)
{
    Parameters result = p;
    // Derived parameters 
    result.ngrid  = static_cast<size_t>((result.x2-result.x1)/result.dx);// number of x points (rounded down)
    result.dt     = 0.5*result.dx/result.c;                             // time step size
    result.nsteps = static_cast<size_t>(result.runtime/result.dt);      // number of steps to reach runtime (rounded down)
    result.nper   = static_cast<size_t>(result.outtime/result.dt);      // how many steps between snapshots (rounded down)
    return result;
}


void create_simulation_system(const Parameters& param, WaveState& wave)
{
    // Define and allocate arrays
    wave.rho_prev = std::make_unique<double[]>(param.ngrid); // time step t-1
    wave.rho      = std::make_unique<double[]>(param.ngrid); // time step t
    wave.rho_next = std::make_unique<double[]>(param.ngrid); // time step t+1
    wave.x        = std::make_unique<double[]>(param.ngrid); // x values
}
