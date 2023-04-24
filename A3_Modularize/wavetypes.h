// wavetypes.h
// defines Parameters and WaveState structures
#ifndef _WAVETYPESH_
#define _WAVETYPESH_

#include <memory>
#include <string>

// create a type that will hold a collection of parameters
class Parameters {
  public:
    double  c;              // wave speed
    double  tau;            // damping time
    double  x1;             // left most x value
    double  x2;             // right most x value
    double  runtime;        // how long should the simulation try to compute?
    double  dx;             // spatial grid size
    double  outtime;        // how often should a snapshot of the wave be written out? 
    std::string outfilename;// name of the file with the output data
    // the remainder are to be derived from the above ones:
    size_t  ngrid;          // number of x points
    double  dt;             // time step size
    size_t  nsteps;         // number of steps of that size to reach runtime
    size_t  nper;           // how many step s between snapshots
};

class WaveState {
  public:
    std::unique_ptr<double[]> rho_prev;
    std::unique_ptr<double[]> rho;
    std::unique_ptr<double[]> rho_next;
    std::unique_ptr<double[]> x;
};

#endif
