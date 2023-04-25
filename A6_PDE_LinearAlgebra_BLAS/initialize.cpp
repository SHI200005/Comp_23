// initialize.cpp
#include "initialize.h"
#include <cmath>

// See initialize.h for how to use the function in this module

void initialize(const Parameters& param, WaveState& wave)
{
    // Initialize array of x values 
    for (size_t i = 0; i < param.ngrid; i++) {
        wave.x[i] = param.x1 + (static_cast<double>(i)*(param.x2-param.x1))/static_cast<double>(param.ngrid-1);
    }
    
    // Initialize wave with a triangle shape from xstart to xfinish
    double xstart = 0.25*(param.x2-param.x1) + param.x1;
    double xmid = 0.5*(param.x2+param.x1);
    double xfinish = 0.75*(param.x2-param.x1) + param.x1;
    for (size_t i = 0; i < param.ngrid; i++) {
        if (wave.x[i] < xstart or wave.x[i] > xfinish) {
            wave.rho[i] = 0.0;
        } else {
            wave.rho[i] = 0.25 - fabs(wave.x[i]-xmid)/(param.x2-param.x1);
        }
        wave.rho_prev[i] = wave.rho[i];
    }

    // Ensure zero Dirichlet boundary conditions
    wave.rho[0] = 0.0;
    wave.rho[param.ngrid-1] = 0.0;
}
