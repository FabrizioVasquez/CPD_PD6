#!/bin/bash
#SBATCH --job-name=bucketsort-pd
#SBATCH -o bucketsort_tiempos.out 

module load gcc/10.1.0
module load mpich/4.0

mpic++ parallel_bucketsort.cpp -o bucketsort.exe

for i in 1 2 4 8 16
do
 mpirun -np $i ./bucketsort.exe
done

module unload mpich/4.0
module unload gcc/10.1.0