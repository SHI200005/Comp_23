// wavetypes.h
// defines Parameters and WaveState structures
#ifndef _WAVETYPESH_
#define _WAVETYPESH_

#include <rarray>
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
    bool    netcdfoutput;   // should output be in netcdf format?
};

class WaveState {
  public:
    rarray<double,2> rho_prev;
    rarray<double,2> rho;
    rarray<double,2> rho_next;
    rarray<double,1> x;
};

#endif
