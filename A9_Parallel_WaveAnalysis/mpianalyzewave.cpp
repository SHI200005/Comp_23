// analyzewave.cpp
//
// Reads a netcdf file produced by wave2d and computes the potential,
// kinetic, and total energy of the wave as a function of time.
//

#include <fstream>
#include <iostream>
#include <netcdf>
#include <rarray>
#include <mpi.h>

using namespace netCDF;

// helper routines to get attributes and dimensions from an open netcdf file
double get_double_attribute(NcFile& f, const std::string& attname)
{
    double result;
    f.getAtt(attname).getValues(&result);
    return result;
}

unsigned long get_dimension(NcFile& f, const std::string& dimname)
{
    return f.getDim(dimname).getSize();
}

int main(int argc, char** argv)
{
    // Program parameters are the name of an input file and an output
    // file and are given from command line or, if not, are given
    // default values "precisewave.nc" and "energies.tsv".
    
    std::string inputfilename((argc<=1)?"precisewave.nc":argv[1]);
    std::string outputfilename((argc<=2)?"energies.tsv":argv[2]);
   
   
    // find this process place
    // Initialize MPI
    MPI_Init(&argc, &argv);
    // Get the rank and size of the current process
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);    
   
   
    //std::cout << "Reading from " << inputfilename << "\n";
    //std::cout << "Writing to " << outputfilename << "\n";

    // Open file and get some of the simulation parameters as well as the
    // number of stored steps in the netcdf file 
    NcFile f(inputfilename, NcFile::read);
    unsigned long nrows     = get_dimension(f, "nxgrid"); 
    unsigned long ncols     = get_dimension(f, "nygrid"); 
    unsigned long noutsteps = get_dimension(f, "noutsteps"); 
    double c       = get_double_attribute(f, "c");
    double dx      = get_double_attribute(f, "dx");
    double outtime = get_double_attribute(f, "outtime");
    double runtime = get_double_attribute(f, "runtime");
    
    
    // determine its subrange of data
    const long nrows_per = (nrows+size-1)/size;
    const long nrows_start = nrows_per*rank;
    const long nrows_own = (rank<size-1)? nrows_per+1 : (nrows-nrows_per*(size-1)); //!!ACHTUNG!!    


    // Find the rho and t variables, and create arrays to hold time slices
    NcVar rho_handle  = f.getVar("rho");
    NcVar time_handle = f.getVar("t");    
    rmatrix<double> rho(nrows_own,ncols);
    rmatrix<double> rho_prev(nrows_own,ncols);
    double time;

    // Start output with a header
    std::ofstream fout(outputfilename);
    fout << "# c="         << c << "\n"
         << "# dx="        << dx << "\n"
         << "# outtime="   << outtime << "\n"
         << "# ngrid="     << nrows << "x" << ncols << "\n"
         << "# noutsteps=" << noutsteps << "\n"
         << "#time\tkinetic-energy\tpotential-energy\ttotal-energy\n";

    // Get first time slice
    rho_handle.getVar({0,nrows_start,0}, {1,nrows_own,ncols}, rho.data()); //!!ACHTUNG!! The starting poniter should be modified!!
    time_handle.getVar({0}, {1}, &time);
    
    //std::cout << "Analyzing total time " << runtime
    //          << " (" << noutsteps << " steps)\n";

    for (unsigned long s = 1; s < noutsteps; s++)
    {
        // store previous slice as rho_prev to be able to compute the
        // time derivative of the wave field below
        std::swap(rho, rho_prev);
        
        // get next time slice
        time_handle.getVar({s}, {1}, &time);
        rho_handle.getVar({s,nrows_start,0}, {1,nrows_own,ncols}, rho.data()); //!!ACHTUNG!! The starting poniter should be modified!!
        
        // report status to console
        std::cout << "\rCurrently analyzing time " << time << " (step " << s+1 << ")    ";
        std::cout.flush();
        
        // compute energies
        double T_own = 0.0; //to avoid that it looks like "Town"
        double V_own = 0.0;
        for (int i=1; i<nrows_own; i++) { // Self-consistent
            for (int j=1; j<ncols; j++) {  // Self-consistent
                T_own += pow((dx/outtime)*(rho[i][j]-rho_prev[i][j]),2);
                V_own += pow(c,2)*(pow(rho[i][j]-rho[i-1][j],2)
                    +pow(rho[i][j]-rho[i][j-1],2));
            }
        }
        
        // Using MPI_Allreduce to sum the local energies together
        double T, V;
        MPI_Allreduce(&T_own, &T, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
        MPI_Allreduce(&V_own, &V, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
        
        // store in output file
        fout << time << "\t" << T << "\t" << V << "\t" << T+V << "\n";
    }
    
    f.close();

    //std::cout << "\nDone\nOutput written to energies.dat\n";
    
    // Finalize MPI
    MPI_Finalize();

}
