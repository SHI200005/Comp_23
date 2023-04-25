// evolve.cpp
//
// See evolve.h for how to use the function in this module
//

#include <cmath>
#include <omp.h>
#include "evolve.h"

void one_time_step(const Parameters& param, WaveState& wave)
{    
    // Evolve inner region over a time dt using a leap-frog variant
    #pragma omp parallel for default(none) shared(param, wave)
    for (size_t i = 1; i <= param.ngrid-2; i++) {
        #pragma omp parallel for default(none) shared(param, wave, i)
        for (size_t j = 1; j <= param.ngrid-2; j++) {
            double laplacian = pow(param.c/param.dx,2)*(wave.rho[i+1][j]
                                                        + wave.rho[i-1][j]
                                                        + wave.rho[i][j+1]
                                                        + wave.rho[i][j-1]
                                                        - 4*wave.rho[i][j]);
            double friction = (wave.rho[i][j] - wave.rho_prev[i][j])/param.tau;
            wave.rho_next[i][j] = 2*wave.rho[i][j] - wave.rho_prev[i][j]
                + param.dt*(laplacian*param.dt-friction);
        }
    }
    
    // Update arrays such that t+1 becomes the new t etc.
    std::swap(wave.rho_prev, wave.rho);
    std::swap(wave.rho, wave.rho_next);
}
