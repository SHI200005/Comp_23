// output.h
#ifndef  _OUTPUTH_
#define  _OUTPUTH_

#include <fstream>
#include "wavetypes.h"

// The 'output_parameters' function writes the parameters in param out
// the an existing, open file stream fout.
// The parameters will be prepended with a '#' sign in the file.
void output_parameters(std::ofstream& fout, const Parameters& param);

// The 'output_wave' function writes the values of rho as a function
// of those of x into an existing, open file stream fout.
//
// The values are prepended by a line of the form '# t = ...',
// with ... replaced by the value of time.
// 'n' should be the number of element of x and rho to write out.
void output_wave(std::ofstream& fout, double time, size_t n, WaveState& wave);

#endif
