* Checking Open MPI Thread Support
$ ompi_info --parseable | grep THREAD
option:threads:posix (MPI_THREAD_MULTIPLE: yes, OPAL support: yes, OMPI progress: no, ORTE progress: yes, Event lib: yes)

* Compile Open MPI with MPI Thread support
./configure --enable-mpi-threads --with-threads=posix --disable-ipv6 
