#!/bin/bash
#SBATCH --job-name=wave2d
#SBATCH --output=wave2d_%j.txt
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --cpus-per-task=16
#SBATCH --time=00:15:00

# Load required modules
module load gcc/12 rarray hdf5 netcdf

make clean && make

# Define the list of thread counts to run
THREADS=(1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16)

# Run the code for each thread count and write the elapsed time to the output file
for i in ${THREADS[@]}
do
    echo "\nRunning with $i threads..."
    ELAPSED_TIME=$(time (export OMP_NUM_THREADS=$i && ./wave2d longwaveparams.txt) 2>&1 >/dev/null | grep "real" | awk '{print $2}')
done

echo "Job completed successfully!"

make clean