dnl Define MPI library (http://www.open-mpi.org)
AC_DEFUN([AC_PROG_MPI],
[
    AC_ARG_WITH(mpi,
            AC_HELP_STRING([--with-mpi=PATH],[path to mpicc]),
    [ if test "$withval" != "yes"; then
          AC_MSG_CHECKING([for MPI])
	  if test -x "$withval" ; then
	      MPI="$withval"
	      AC_MSG_RESULT([$MPI])
	  else
	      MPI="no"
	      AC_MSG_RESULT(no)
	  fi
      fi
     ])
     
     if test -z "$MPI"; then
         AC_PATH_PROG([MPI], [mpicxx], [no])
     fi

     if test $MPI = no; then
        AC_MSG_ERROR([MPI could no be found on your system.
		      You can download MPI for free from
		      http://www.open-mpi.org
                      Please check that MPI is installed on your system
		      and your path includes the top-level install directory
		      (example: /usr/include/openmpi), or specify
		      --with-mpi=PATH option when running configure.])
     fi
     AC_SUBST(MPI)
])

dnl Check if mpicxx requires -lstdc++ option 
AC_DEFUN([AC_LINK_STDCPP],
[
     AC_MSG_CHECKING([if $CXX needs stdc++])
     AC_LANG([C++])
     AC_LANG_CONFTEST()
     
     AC_LINK_IFELSE(
         [AC_LANG_PROGRAM([[#include "mpi.h"]],[[std::cout << "Hello";]])],
         [AC_MSG_RESULT([no])
	     STDCPP=""
	 ],
	 [AC_MSG_RESULT([yes])
	     STDCPP="-lstdc++"
	 ])

     AC_SUBST(STDCPP)
])
