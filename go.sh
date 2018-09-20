#!/bin/bash
#SBATCH --time=1:0:00
#SBATCH --job-name=centrality-solver
#SBATCH --nodes=2
#SBATCH --ntasks=2
#SBATCH --ntasks-per-node=1
#SBATCH --cpus-per-task=2
#SBATCH --partition=coursework

export OMP_NUM_THREADS=${SLURM_CPUS_PER_TASK}

DATE=$(date +"%Y%m%d%H%M")
echo "time started  "$DATE

time mpirun -n 2 ./centrality-solver test/test5

DATE=$(date +"%Y%m%d%H%M")
echo "time finished "$DATE
