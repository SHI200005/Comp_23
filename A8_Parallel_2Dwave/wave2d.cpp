// wave1d.cpp - Simulates a one-dimensional damped wave equation
//
// Ramses van Zon - 2015-2023
//

#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <cmath>

#include "parameters.h"
#include "initialize.h"
#include "output.h"
#include "evolve.h"
#include "simulation.h"

//using namespace 

int main(int argc, char* argv[])
{
    if (argc != 2) {
        std::cerr << "Error: wave1d needs one parameter file argument.\n";
        return 1;
    }

    // Read the values from the parameter file specified on the command line
    // but exit with error code if something went wrong
    Parameters param;
    int errorcode = readParametersFromFile(argv[1], param);
    if (errorcode > 0) 
        return errorcode;
    
    // Derived parameters 
    param = set_derived_simulation_parameters(param);
    
    // Open output file
    ascii_file_handle  ascf{};
    netcdf_file_handle ncdf{nullptr};
    if (param.netcdfoutput)
        ncdf = netcdf_output_open(param.outfilename);
    else
        ascf = ascii_output_open(param.outfilename);

    output_parameters(ascf, param);
    output_parameters(ncdf, param);
    
    // Define state
    WaveState wave;
    create_simulation_system(param, wave);
    initialize(param, wave);

    // Output initial wave to file
    output_wave(ascf, 0.0, wave);
    output_wave(ncdf, 0.0, wave);

    // Take timesteps
    for (size_t s = 0; s < param.nsteps; s++) {
        
        // Evolve in time
        one_time_step(param, wave);
        
        // Output wave to file
        if ((s+1)%param.nper == 0) {
            double t = static_cast<double>(s+1)*param.dt;
            output_wave(ascf, t, wave);
            output_wave(ncdf, t, wave);
        }
    }

    // Close file
    output_close(ascf);
    output_close(ncdf);
    std::cout << "Results written to '"<< param.outfilename << "'.\n";

    return 0;
}

