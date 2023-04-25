#!/bin/bash
#SBATCH --job-name=mpianalyzewave
#SBATCH --output=mpianalyzewave_%j.txt
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=16
#SBATCH --cpus-per-task=1
#SBATCH --time=01:15:00

# Load required modules
module load gcc/12 openmpi rarray gdb hdf5 netcdf

make clean && make

INPUT_FILE="precisewave.nc"
OUTPUT_FILE="energies.tsv"
EXECUTABLE="./mpianalyzewave"

# Define the list of thread counts to run
NUM_PROCESSORS=(1 2 3 5 9 16)

# Create a table to store the timings
echo "Processes,Time (s)" > scaling_analysis.txt

# Loop over the number of processes and run the analysis
for i in "${NUM_PROCESSORS[@]}"
do
    echo "Running with $i processes"
    echo -n "$i," >> scaling_analysis.txt
    (time mpirun -n $i $EXECUTABLE $INPUT_FILE $OUTPUT_FILE) 2>&1 | grep "real" | awk '{print $2}' >> scaling_analysis.txt
done

echo "Job completed successfully!"

make clean
