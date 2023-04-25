// output.h - ASCII and NetCDF output routines
#ifndef  _OUTPUTH_
#define  _OUTPUTH_

#include <fstream>
#include <memory>
#include <netcdf>
#include "wavetypes.h"


// ASCII output 
using ascii_file_handle=std::ofstream;

// Open a text file and return the file handle
ascii_file_handle ascii_output_open(std::string filename);

// The 'output_parameters' function writes the parameters in param out
// the an existing, open file handle.
// The parameters will be prepended with a '#' sign in the file.
void output_parameters(ascii_file_handle& handle, const Parameters& param);

// The 'output_wave' function writes the values of rho as a function
// of those of x into an existing, open file handle.
//
// The values are prepended by a line of the form '# t = ...',
// with ... replaced by the value of time.
void output_wave(ascii_file_handle& handle, double time, WaveState& wave);

// Close a text file by file handle
void output_close(ascii_file_handle& handle);


using netcdf_file_handle = std::unique_ptr<netCDF::NcFile>;

// Open a netcdf file and return the file handle
netcdf_file_handle netcdf_output_open(std::string filename);

// The 'output_parameters' function writes the parameters in param out
// the an existing, open netcdf file handle.
void output_parameters(netcdf_file_handle& handle, const Parameters& param);

// The 'output_wave' function writes the values of rho.
// The values of x are stored when called for the first time.
// The values of times are stored in a separate array
void output_wave(netcdf_file_handle& handle, double time, WaveState& wave);

// Close a netcdf file by a file handle
void output_close(netcdf_file_handle& handle);


#endif

