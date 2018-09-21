#!/bin/bash
#SBATCH --time=1:0:00
#SBATCH --job-name=centrality-solver
#SBATCH --nodes=2
#SBATCH --ntasks=2
#SBATCH --ntasks-per-node=1
#SBATCH --cpus-per-task=2
#SBATCH --partition=cosc
#SBATCH --mem 4000

export OMP_NUM_THREADS=${SLURM_CPUS_PER_TASK}

DATE=$(date +"%Y%m%d%H%M")
echo "time started  "$DATE

#module load openmpi-x86_64
mpirun -n 2 ./centrality-solver test/complete_5000.graph

DATE=$(date +"%Y%m%d%H%M")
echo "time finished "$DATE
