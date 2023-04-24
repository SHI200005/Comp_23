// evolve.cpp
#include <cmath>
#include "evolve.h"

// See evolve.h for how to use the function in this module

void one_time_step(const Parameters& param, WaveState& wave)
{    
    // Evolve inner region over a time dt using a leap-frog variant
    for (size_t i = 1; i <= param.ngrid-2; i++) {
        double laplacian = pow(param.c/param.dx,2)*(wave.rho[i+1] + wave.rho[i-1] - 2*wave.rho[i]);
        double friction = (wave.rho[i] - wave.rho_prev[i])/param.tau;
        wave.rho_next[i] = 2*wave.rho[i] - wave.rho_prev[i] + param.dt*(laplacian*param.dt-friction);
    }
    
    // Update arrays such that t+1 becomes the new t etc.
    std::swap(wave.rho_prev, wave.rho);
    std::swap(wave.rho, wave.rho_next);
}
