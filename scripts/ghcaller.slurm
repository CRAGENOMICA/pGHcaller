#!/bin/bash 
#

source /opt/Modules/3.2.9/init/Modules4bash.sh 
module load openmpi/1.6.5 gcc/4.9.1

# TODO log functions


# TODO Get options and check environment
echo "[GHCALLER] Checking environment"

# TODO Test pghcaller binary
PGHCALLER=pghcaller

# TODO Test mpirun
MPIRUN=`which mpirun`
if [[ -z "$MPIRUN" ]] || [[ ! -x "$MPIRUN" ]];
then
	echo "ERROR cannot locate mpirun binary"	
else
	echo "[GHCALLER] mpirun binary: $MPIRUN"
fi

# Get Nodes and calculate number of threads
NODES=$SLURM_NNODES
THREADS=$SLURM_NTASKS_PER_SOCKET
NPROCS=$SLURM_NPROCS
CPUSPERNODE=2

echo "[GHCALLER] Run application using $NODES nodes and $THREADS threads-per-node"

# TODO Run some things
printenv

# Run pGHcaller in out environment
T="$(date +%s)"
INIT=$T
echo "Running GHcaller mpileup (t=$T)"
args=$@
echo "[GHCALLER] Running ARGS $args"
export OMP_NUM_THREADS=$THREADS
#mpirun -x OMP_NUM_THREADS  -display-map -report-bindings -npersocket 1 -bind-to-socket -np $((NODES*CPUSPERNODE)) $PGHCALLER $args
mpirun -x OMP_NUM_THREADS  -display-map -report-bindings -np $((NODES*CPUSPERNODE)) $PGHCALLER $args

T="$(($(date +%s)-T))"
echo "# PGHCALLER $CHR Time_in_seconds: ${T} Tasks: $SLURM_NTASKS"
printf "Pretty format: %02d:%02d:%02d:%02d\n" "$((T/86400))" "$((T/3600%24))" "$((T/60%60))" "$((T%60))"


# Exit
echo "[GHCALLER] Done"
exit 0 
