// initialize.cpp
#include "initialize.h"
#include <cmath>
#include <omp.h>

// See initialize.h for how to use the function in this module

double p(double x, double x1, double x2)
{
    double xstart = 0.25*(x2-x1) + x1;
    double xmid = 0.5*(x2+x1);
    double xfinish = 0.75*(x2-x1) + x1;
    if (x < xstart or x > xfinish) 
        return 0.0;
    else 
        return (0.25 - fabs(x-xmid)/(x2-x1));
}

void initialize(const Parameters& param, WaveState& wave)
{
    // Initialize array of x values 
    for (size_t i = 0; i < param.ngrid; i++) {
        wave.x[i] = param.x1 + (static_cast<double>(i)*(param.x2-param.x1))/static_cast<double>(param.ngrid-1);
    }
    
    // Initialize wave with a triangle shape from xstart to xfinish
    #pragma omp parallel for default(none) shared(param, wave)
    for (size_t i = 0; i < param.ngrid; i++) {
        #pragma omp parallel for default(none) shared(param, wave, i)
        for (size_t j = 0; j < param.ngrid; j++) {
            double x = wave.x[i];
            double y = wave.x[j];
            wave.rho[i][j] = p(x, param.x1, param.x2) * p(y, param.x1, param.x2);
            wave.rho_prev[i][j] = wave.rho[i][j];
        }
    }

    // Ensure zero Dirichlet boundary conditions
    for (size_t i = 0; i < param.ngrid; i++) {
        wave.rho[0][i] = 0.0;
        wave.rho[param.ngrid-1][i] = 0.0;
        wave.rho[i][0] = 0.0;
        wave.rho[i][param.ngrid-1] = 0.0;
    }
}
