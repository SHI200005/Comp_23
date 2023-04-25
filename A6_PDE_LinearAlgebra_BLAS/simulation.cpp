// simulation.cpp
#include "simulation.h"
#include <tuple>
#include <rarray>
#include <cmath>

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
    wave.rho_prev = rarray<double,1>(param.ngrid); // time step t-1
    wave.rho      = rarray<double,1>(param.ngrid); // time step t
    // wave.rho_next = rarray<double,1>(param.ngrid); // time step t+1
    wave.x        = rarray<double,1>(param.ngrid); // x values
}

LinearAlgebra set_Linear_Algebra_Formulation(const Parameters& param, LinearAlgebra& matrix)
{
    // Define the matrix and scalar
    matrix.beta = param.dt/param.tau -1;
    
    
    matrix.A    = rarray<double,2>(param.ngrid,param.ngrid);
    // calculate the matrix A
    // with my definition, I don't need boundary conditions, they are implied in the matrix A
    double diagonal = 2 - 2*pow(param.dt*param.c/param.dx,2) - param.dt/param.tau;
    double adjacent = pow(param.dt*param.c/param.dx,2);
    matrix.A.fill(0.0);
    for (int i=1; i<param.ngrid-1; i++) {
      matrix.A[i][i-1] = adjacent;
      matrix.A[i][i] = diagonal;
      matrix.A[i][i+1] = adjacent;
    }
    // std::cout << matrix.beta << std::endl;
    // std::cout << matrix.A    << std::endl;
    return matrix;
}