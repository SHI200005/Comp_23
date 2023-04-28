# Assignment 8: Parallel two-dimensional wave equation

In this [assignment](https://education.scinet.utoronto.ca/mod/assign/view.php?id=2343), you will parallelize a given two-dimensional damped wave application wave2d (e.g., the partial solution to the 4th assignment), and assess its scaling performance. Note that this code also implements output to netCDF if the output filename has extension ".nc".

In this folder, the result file was too large that I didn't upload it.

## Work in SCRATCH directory

Before, you are working in **home directory**, on the **log in node**. Now you submit your job to run it on **compute nodes**. You will need to write the results of your program after running it. However, on teach cluster or Niagara, the home directories are read only. To have the results written, you need to work in **scratch directory**. Before starting your homework, type

```
cd $SCRATCH
```

to switch to scratch directory. To see the address, type

```
pwd
```

and you will see where you are.

If you want to go back to home directory, type

```
cd $HOME
```

## Parallelize with OpenMP

Two most time demanding 'for loop' parts were made parallelized. They will decide how to chop automatically.

1. in function 'initialize' in 'initilize.cpp'

   ```
   void initialize(const Parameters& param, WaveState& wave)
   {
       // Initialize array of x values 
       for (size_t i = 0; i < param.ngrid; i++) {
           wave.x[i] = param.x1 + (static_cast<double>(i)*(param.x2-param.x1))/static_cast<double>(param.ngrid-1);
       }
       
       // Initialize wave with a triangle shape from xstart to xfinish
       #pragma omp parallel for default(none) shared(param, wave)
       for (size_t i = 0; i < param.ngrid; i++) {
           #pragma omp parallel for default(none) shared(param, wave, i)
           for (size_t j = 0; j < param.ngrid; j++) {
               double x = wave.x[i];
               double y = wave.x[j];
               wave.rho[i][j] = p(x, param.x1, param.x2) * p(y, param.x1, param.x2);
               wave.rho_prev[i][j] = wave.rho[i][j];
           }
       }
   
       // Ensure zero Dirichlet boundary conditions
       for (size_t i = 0; i < param.ngrid; i++) {
           wave.rho[0][i] = 0.0;
           wave.rho[param.ngrid-1][i] = 0.0;
           wave.rho[i][0] = 0.0;
           wave.rho[i][param.ngrid-1] = 0.0;
       }
   }
   ```

2. in function 'one_time_step' in 'evolve.cpp'

   ```
   void one_time_step(const Parameters& param, WaveState& wave)
   {    
       // Evolve inner region over a time dt using a leap-frog variant
       #pragma omp parallel for default(none) shared(param, wave)
       for (size_t i = 1; i <= param.ngrid-2; i++) {
           #pragma omp parallel for default(none) shared(param, wave, i)
           for (size_t j = 1; j <= param.ngrid-2; j++) {
               double laplacian = pow(param.c/param.dx,2)*(wave.rho[i+1][j]
                                                           + wave.rho[i-1][j]
                                                           + wave.rho[i][j+1]
                                                           + wave.rho[i][j-1]
                                                           - 4*wave.rho[i][j]);
               double friction = (wave.rho[i][j] - wave.rho_prev[i][j])/param.tau;
               wave.rho_next[i][j] = 2*wave.rho[i][j] - wave.rho_prev[i][j]
                   + param.dt*(laplacian*param.dt-friction);
           }
       }
       
       // Update arrays such that t+1 becomes the new t etc.
       std::swap(wave.rho_prev, wave.rho);
       std::swap(wave.rho, wave.rho_next);
   }
   ```

   

In order to use OpenMP, don't forget

```
#include <omp.h>
```

in your corresponding .cpp files.

Link to libraries in your Makefile

```
LDLIBS=-lnetcdf -lnetcdf_c++4 -pg -g -gdwarf-2 -fopenmp
```

## SBTCH file to submit your job

Attention: Do not add extra spaces in the bash file!!!

The information for the scheduler:

```
#!/bin/bash
#SBATCH --job-name=wave2d
#SBATCH --output=wave2d_%j.txt
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --cpus-per-task=16
#SBATCH --time=00:15:00
```

The output of the job will be reported in a file 'wave2d_\<jobID\>.txt' (i.e. wave2d_262432.txt in this folder). We are requiring one node, and 16 cores on that node. For one time step, the 16 cores will compute the next time step parallelly. After this time step calculation finished, the 16 cores will compute the next time step. The time we estimate to run this job is 15 min. If it takes more, it will be kicked off.

To run this bash file, we should type (I cannot remember whether we should make it executable by changing the permissions...)

```
sbatch myjob.sh
```

Then there will be a job ID for your job on the console.

To check the states of your jobs, type

```
squeue --me
```

