// output.cpp - ASCII and NetCDF output routines
// See output.h for how to use the functions in this module

#include "output.h"


ascii_file_handle ascii_output_open(std::string filename)
{
    return ascii_file_handle(filename);
}

void output_parameters(ascii_file_handle& handle, const Parameters& param)
{
    // Report all the parameters in the output file (prepend # to
    // facilitate post-processing, as e.g. gnuplot and numpy.loadtxt
    // skip these)
    if (handle) {
        handle << "#c        " << param.c       << "\n";
        handle << "#tau      " << param.tau     << "\n";
        handle << "#x1       " << param.x1      << "\n";
        handle << "#x2       " << param.x2      << "\n";
        handle << "#runtime  " << param.runtime << "\n";
        handle << "#dx       " << param.dx      << "\n";
        handle << "#outtime  " << param.outtime << "\n";
        handle << "#filename " << param.outfilename << "\n"; 
        handle << "#ngrid (derived) " << param.ngrid  << "\n";
        handle << "#dt    (derived) " << param.dt     << "\n";
        handle << "#nsteps(derived) " << param.nsteps << "\n";    
        handle << "#nper  (derived) " << param.nper   << "\n";
    }
}

void output_wave(ascii_file_handle& handle, double time, WaveState& wave)
{
    if (handle) {
        // Output wave to file
        if (time == 0.0)
            handle << "\n#";
        else
            handle << "\n\n# ";
        handle << "t = " << time << "\n";
        size_t n = std::min(wave.rho.extent(0),wave.x.extent(0));
        size_t m = std::min(wave.rho.extent(1),wave.x.extent(0));
        for (size_t i = 0; i < n; i++)  {
            for (size_t j = 0; j < m; j++)  {
                handle << wave.x[i] << " " << wave.x[j] << " " << wave.rho[i][j] << "\n";
            }
        }
    }
}

// Close a file by file handle
void output_close(ascii_file_handle& handle)
{
    if (handle) {
        handle.close();
    }
}


netcdf_file_handle netcdf_output_open(std::string filename)
{
    // Open a netcdf file and return the file handle
    return std::make_unique<netCDF::NcFile>(filename, netCDF::NcFile::replace);
}

void output_parameters(netcdf_file_handle& handle, const Parameters& param)
{
    // The 'output_parameters' function writes the parameters in param out
    // the an existing, open netcdf file handle.
    // The parameters will be prepended with a '#' sign in thenetcdf file.
    if (handle) {
        handle->putAtt("c",       netCDF::ncDouble, param.c);
        handle->putAtt("tau",     netCDF::ncDouble, param.tau);
        handle->putAtt("x1",      netCDF::ncDouble, param.x1);
        handle->putAtt("x2",      netCDF::ncDouble, param.x2);
        handle->putAtt("runtime", netCDF::ncDouble, param.runtime);
        handle->putAtt("dx",      netCDF::ncDouble, param.dx);
        handle->putAtt("outtime", netCDF::ncDouble, param.outtime);
        handle->putAtt("filename", param.outfilename);
        handle->putAtt("dt",      netCDF::ncDouble, param.dt);
        handle->putAtt("nsteps",  netCDF::ncInt64, (long)param.nsteps);
        handle->putAtt("nper",    netCDF::ncInt64, (long)param.nper);
        netCDF::NcDim dimXGrid    = handle->addDim("nxgrid", param.ngrid);
        netCDF::NcDim dimYGrid    = handle->addDim("nygrid", param.ngrid);
        netCDF::NcDim dimOutSteps = handle->addDim("noutsteps");
        handle->addVar("x",   netCDF::ncDouble, dimXGrid);
        handle->addVar("y",   netCDF::ncDouble, dimYGrid);
        handle->addVar("t",   netCDF::ncDouble, dimOutSteps);
        handle->addVar("rho", netCDF::ncDouble, {dimOutSteps,dimXGrid,dimYGrid});
    }
}

void output_wave(netcdf_file_handle& handle, double time, WaveState& wave)
{
    // The 'output_wave' function writes the values of rho.
    // The values of x and y are stored when called for the first time.
    // The values of times are stored in a separate array
    if (handle) {
        const size_t s = handle->getDim("noutsteps").getSize();
        if (s==0) {
            handle->getVar("x").putVar(wave.x.data());            
            handle->getVar("y").putVar(wave.x.data());            
        }
        handle->getVar("t").putVar( {s}, {1},&time ); 
        handle->getVar("rho").putVar 
            ( {s, 0 ,0},
              {1, wave.rho.extent(0), wave.rho.extent(1)},
              wave.rho.data() );
    }
}

void output_close(netcdf_file_handle& handle)
{
    // Close a netcdf file by a file handle
    if (handle) {
        handle.reset(nullptr);
    }
}

