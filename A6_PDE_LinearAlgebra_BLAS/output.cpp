// output.cpp
#include "output.h"

// See output.h for how to use the functions in this module

void output_parameters(std::ofstream& fout, const Parameters& param)
{
    // Report all the parameters in the output file (prepend # to
    // facilitate post-processing, as e.g. gnuplot and numpy.loadtxt
    // skip these)
    fout << "#c        " << param.c       << "\n";
    fout << "#tau      " << param.tau     << "\n";
    fout << "#x1       " << param.x1      << "\n";
    fout << "#x2       " << param.x2      << "\n";
    fout << "#runtime  " << param.runtime << "\n";
    fout << "#dx       " << param.dx      << "\n";
    fout << "#outtime  " << param.outtime << "\n";
    fout << "#filename " << param.outfilename << "\n"; 
    fout << "#ngrid (derived) " << param.ngrid  << "\n";
    fout << "#dt    (derived) " << param.dt     << "\n";
    fout << "#nsteps(derived) " << param.nsteps << "\n";    
    fout << "#nper  (derived) " << param.nper   << "\n";
}

void output_wave(std::ofstream& fout, double time, size_t n, WaveState& wave)
{
    // Output wave to file
    if (time == 0.0)
        fout << "\n#";
    else
        fout << "\n\n# ";
    fout << "t = " << time << "\n";
    for (size_t i = 0; i < n; i++)  {
        fout << wave.x[i] << " " << wave.rho[i] << "\n";
    }
}
