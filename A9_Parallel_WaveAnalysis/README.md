# Assignment 9: Parallelize NetCDF data analysis

The wave2d application of the previous assignment has been used to produce a large NetCDF file "precisewave.nc" containing a time sequence of snapshots of the damped two-dimensional wave at a high resolution, along with all relevent parameters. 

What we are after now is to compute the overall potential energy $V$ and the overall kinetic energy $T$ of the wave field $ρ$, as well as the sum of these two energies $E=T+V$, as a function of time.

In this folder, the "precisewave.nc" file was too large that I didn't upload it.

The 'analyzewave.cpp' is the program before parallelization. The 'mpianalyzewave.cpp'

 is the parallel program, with the corresponding 'job.sh'.

In 'analyzewave.cpp', I think there are some issues with the given code, but I was too busy to talk about it at that time. I put the original code in comments. The reason I is showed below:

<img src="\A9_Parallel_WaveAnalysis\grid.jpg" alt="grid" style="zoom:20%;" />

In the previous code, only the potential energy within the dashed red lines were calculated. However, there were some non-zero values not included.

## Decomposing the Huge 2D Grid

In 'mpianalyzewave.cpp', this corresponds to decomposing

```
#include <mpi.h>

// Initialize MPI
MPI_Init(&argc, &argv);
// Get the rank and size of the current process
int rank, size;
MPI_Comm_rank(MPI_COMM_WORLD, &rank);
MPI_Comm_size(MPI_COMM_WORLD, &size); 
...
// determine its subrange of data
const long nrows_per = (nrows+size-1)/size;
const long nrows_start = nrows_per*rank;
const long nrows_own = (rank<size-1)? nrows_per+1 : (nrows-nrows_per*(size-1)); //!!ACHTUNG!! 
...
// Using MPI_Allreduce to sum the local energies together
double T, V;
MPI_Allreduce(&T_own, &T, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
MPI_Allreduce(&V_own, &V, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
```

The local $T_{own}$ and $V_{own}$ were summed together and reported to all processors.

The following figure shows the idea of chopping the 2D grid. You need to calculate how to chop it given the number of processors.

<img src="\A9_Parallel_WaveAnalysis\chop.jpg" alt="grid" style="zoom:20%;" />

