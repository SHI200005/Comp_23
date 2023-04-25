#!/bin/bash
#SBATCH --job-name=mzasolve
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=16
#SBATCH --cpus-per-task=1
#SBATCH --time=02:15:00

# load the modules needed
module load gcc/12
module load gnu-parallel

# Define the range of Y(0) and Z(0) values
y0_values=($(seq 0.00 0.02 1.00))
z0_values=($(seq 0.00 0.02 1.00))

# Define the output directory for each parameter combination
output_dir="./output"

# Create the output directory and files for storing the results
mkdir -p ${output_dir}

# Define the command to run for each parameter combination
command="mkdir -p ${output_dir}/output_y0_{1}_z0_{2} && \
         ./mzasolve {1} {2} ${output_dir}/output_y0_{1}_z0_{2}/timeseries_y0_{1}_z0_{2}.nc > ${output_dir}/output_y0_{1}_z0_{2}/output_y0_{1}_z0_{2}.txt"

# Use GNU Parallel to run the command for all combinations of Y(0) and Z(0) values,
# with one job per core, and redirect the console output to a file in the output directory
parallel --bar --jobs 16 --joblog joblog.txt "$command" ::: "${y0_values[@]}" ::: "${z0_values[@]}"