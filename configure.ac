#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

#AC_PREREQ([2.69])
AC_PREREQ([2.54])
#AC_INIT([FULL-PACKAGE-NAME], [VERSION], [BUG-REPORT-ADDRESS])
AC_INIT([GHcaller], [0.2.2], [javier.navarro@cragenomica.es])
AC_CONFIG_SRCDIR([src/CmdArguments.h])
AM_INIT_AUTOMAKE([foreign parallel-tests color-tests])
#AM_INIT_AUTOMAKE
AC_CONFIG_HEADERS([config.h])
#AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT
AC_CONFIG_MACRO_DIR([m4])
AC_REQUIRE_CPP

#
# Platform specific setup
#
#############################
AC_CANONICAL_HOST
# Check for which host we are on and setup a few things
# specifically based on the host
case $host_os in
  darwin* )
        # Do something specific for mac
        ;;
  linux*)
        # Do something specific for linux
        ;;
   *BSD*)
        # Do something specific for BSD
        ;;
    *)
        #Default Case
        AC_MSG_ERROR([Your platform is not currently supported])
        ;;
esac


# The macros above automatically fill in some CXXFLAGS so we'll undo it.
AC_LANG(C++)
CXX="mpic++"
CFLAGS=""
CXXFLAGS=""
CPPFLAGS=""

# Checks for programs.
#AC_PROG_CXX_MPI
#AC_PROG_CC([mpic++])
#AC_PROG_CXX
#ACX_MPI
#AC_PROG_CPP
AC_PROG_CXX	# Check for G++
AC_MSG_CHECKING([Properties of MPI compiler])
AC_MSG_RESULT([$CXX])
#AC_LINK_STDCPP
#AC_OPENMP	# Check for OpenMP
#AC_MSG_CHECKING([OpenMP compiler flags])
#AC_MSG_RESULT([$OPENMP_CXXFLAGS])
#ac_pkg_openmp=no
#if test -n "${OPENMP_CXXFLAGS}"; then
#  AC_MSG_CHECKING([whether OpenMP will work in a package])
#  AC_LANG_CONFTEST(
#  [AC_LANG_PROGRAM([[#include <omp.h>]], [[ return omp_get_num_threads (); ]])])
#  PKG_CFLAGS="${OPENMP_CXXFLAGS}" PKG_LIBS="${OPENMP_CXXFLAGS}" "$RBIN" CMD SHLIB conftest.c 1>&AS_MESSAGE_LOG_FD 2>&AS_MESSAGE_LOG_FD && "$RBIN" --vanilla -q -e "dyn.load(paste('conftest',.Platform\$dynlib.ext,sep=''))" 1>&AS_MESSAGE_LOG_FD 2>&AS_MESSAGE_LOG_FD && ac_pkg_openmp=yes
#  AC_MSG_RESULT([${ac_pkg_openmp}])
#fi

# if ${ac_pkg_openmp} = "yes" then we have OMP, otherwise it will be "no"
#if test "${ac_pkg_openmp}" = no; then
#  OPENMP_CXXFLAGS=''
#  # you could put AC_MSG_ERROR here is OpenMP is required
#fi
if test "x${OPENMP_CXXFLAGS}" = "x" ; then
  OPENMP_CXXFLAGS=''
  # you could put AC_MSG_ERROR here is OpenMP is required
  # AC_MSG_WARN(["openmp support is disabled"])
  AC_MSG_WARN(["openmp support is disabled, but it does not matter because is not used"])
fi
#aC_SUBST(AM_CXXFLAGS,"$OPENMP_CXXFLAGS")
AC_SUBST(openmp_CFLAGS, "$OPENMP_CXXFLAGS")


#AC_PROG_ICC
#AC_PROG_RANLIB
#AC_PROG_CC([mpicc])
# If you are developing using C++ bindings of MPI, you'll need an MPI C++ compiler. 
# Autotools will search for this given the AC_PROG_CXX macro in configure.ac.
#AC_PROG_CC([mpic++])
#CC="$MPICC" #OR# 
#CXX="$MPICXX" #OR# F77="$MPIF77"
#CXX="$MPICXX" 
#LIBS="$MPILIBS $LIBS"


# Checks for libraries.
#AC_CHECK_LIB([pthread], [pthread_create])
AC_CHECK_LIB([m],[asin],,,[-lstdc++])
AC_CHECK_LIB([mpi],[MPI_Init],,,[-lstdc++])
AC_CHECK_LIB([readline],[readline],,,[-lstdc++])
#AC_SEARCH_LIBS([gethostname], [c,nsl])
#AC_CHECK_LIB([nsl],[gethostname])
#AC_SEARCH_LIBS([bind],[c,nsl,socket])

# Checks for header files.
# EX: AC_CHECK_HEADERS([boost/foreach.hpp])
AC_HEADER_STDC
AC_CHECK_HEADERS([fcntl.h stdlib.h string.h unistd.h mpi.h])

# Checks for typedefs, structures, and compiler characteristics.
# AC_CHECK_HEADER_STDBOOL
#AC_TYPE_INT8_T
AC_C_INLINE
AC_FUNC_ERROR_AT_LINE
AC_OPENMP
AX_CXX_COMPILE_STDCXX_11( ,[mandatory])


# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_MMAP
AC_CHECK_FUNCS([munmap])
#AC_FUNC_MEMCMP
#AC_TYPE_SIGNAL
#AC_FUNC_STRTOD
#AC_CHECK_FUNCS([bzero floor gettimeofday memset mkdir modf pow rint sqrt strcasecmp strchr strdup strstr strtol])

# Enable debug
# AC_ARG_ENABLE([debug],
#[  --enable-debug    Turn on debugging and compile with debug symbols],
#[case "${enableval}" in
#  yes) debug=true ;;
#  no)  debug=false ;;
#  *) AC_MSG_ERROR([bad value ${enableval} for --enable-debug]) ;;
#esac],[debug=false])
#AM_CONDITIONAL([DEBUG], [test x$debug = xtrue])

AC_ARG_ENABLE(debug,
AS_HELP_STRING([--enable-debug],
               [enable debug, default: no]),
	[case "${enableval}" in
		yes) debug=true ;;
		no)  debug=false ;;
		*)   AC_MSG_ERROR([bad value ${enableval} for --enable-debug]) ;; 
	esac],[coverage=false])
AM_CONDITIONAL(SETDEBUG, test x"$debug" = x"true")
AM_COND_IF([SETDEBUG],
    AC_DEFINE(HAVE_DEBUG, 1, [Define to 0 if this is a release build]),
    AC_DEFINE(HAVE_DEBUG, 0, [Define to 1 or higher if this is a debug build]))

# Release 
#AC_ARG_ENABLE(release,
#	AC_HELP_STRING([--enable-release],
#		[Use 'Release' Configuration [default=NO]]),
#		enable_release=yes, enable_release=no)
#AM_CONDITIONAL(ENABLE_RELEASE, test x$enable_release = xyes)
#if test "x$enable_release" = "xyes" ; then
#	RELEASE_CONFIG_LIBRARIES=''
#	RELEASE_CONFIG_LIBS=''
#	AC_SUBST(RELEASE_CONFIG_LIBRARIES)
#	AC_SUBST(RELEASE_CONFIG_LIBS)
#	CONFIG_REQUESTED="yes"
#fi
 

# Enable code profiling
AC_ARG_ENABLE(profile,  AS_HELP_STRING([--enable-profile],  [enable code profiling]),
		[case "${enable_profile}" in
		  yes) profile=true  ;;
		  no)  profile=false ;;
		  *) AC_MSG_ERROR([bad value ${enable_profile} for --enable-profile]) ;;
		esac],
		[profile=false])
# add CXXFLAGS="-pg"
AM_CONDITIONAL([PROFILE], [test x"$profile" = x"true"])
AM_COND_IF(PROFILE,
    AC_DEFINE(PROFILE, 1, [Define to 0 if this is a release build]),
    AC_DEFINE(PROFILE, 0, [Define to 1 or higher if this is a debug build]))

# Run the test-output.txt(.in) through autoconf substitution logic.
#AC_OUTPUT(test-output.txt)

# Enable event logging
AC_ARG_ENABLE(mpe,  AS_HELP_STRING([--enable-mpe],  [enable mpe profiling]),
		[case "${enable_mpe}" in
		  yes) mpe=true  ;;
		  no)  mpe=false ;;
		  *) AC_MSG_ERROR([bad value ${enable_mpe} for --enable-mpe]) ;;
		esac],
		[mpe=false])

# Check libs and set CXX
if test x"$mpe" = x"true" ; then
    # TODO: Check mpecc, libs and SUBST
    # add CXXFLAGS="-pg"
    CXX="mpecc"
fi
AM_CONDITIONAL([MPE], [test x"$mpe" = x"true"])
AM_COND_IF(MPE,
    AC_DEFINE(MPE, 1, [Define to 0 if this is a release build]),
    AC_DEFINE(MPE, 0, [Define to 1 or higher if this is a debug build]))

#AC_ARG_ENABLE([eventlog],  AS_HELP_STRING([--enable-eventlog],  [enable event log functions]),
#		[case "${enable_eventlog}" in
#		  mpe) eventlog=mpe ;;
#		  time)  eventlog=time ;;
#		  *) AC_MSG_ERROR([bad value ${enable_eventlog} for --enable-eventlog. Posible values are: mpe, time]) ;;
#		esac],
#		[eventlog=false])
#AM_CONDITIONAL([ENABLEDEBUG], [test x$debug = xtrue])
## add CXXFLAGS="$CXXFLAGS -g3 -O0 -Wall -Werror -DDEBUG=1"
#AM_COND_IF(ENABLEDEBUG,
#    AC_DEFINE(ENABLEDEBUG, 1, [Define to 0 if this is a release build]),
#    AC_DEFINE(ENABLEDEBUG, 0, [Define to 1 or higher if this is a debug build]))
#
dnl ---------------------------------------------------------------------------
dnl Checks for additional header files.
AC_CHECK_HEADERS([getopt.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_TYPE_SIZE_T
AC_HEADER_TIME
AC_C_VOLATILE

# Checks for library functions.
AC_HEADER_STDC
AC_FUNC_SELECT_ARGTYPES
AC_CHECK_FUNCS([getopt_long])


######################################################################
# DOXYGEN SUPPORT
######################################################################

DX_HTML_FEATURE(ON)
DX_CHM_FEATURE(OFF)
DX_CHI_FEATURE(OFF)
DX_MAN_FEATURE(OFF)
DX_RTF_FEATURE(OFF)
DX_XML_FEATURE(OFF)
DX_PDF_FEATURE(OFF)
DX_PS_FEATURE(OFF)

DX_INIT_DOXYGEN([$PACKAGE_NAME],[doxygen.cfg])

######################################################################

# Output
AC_OUTPUT(
	Makefile 
	src/Makefile 
	doc/Makefile 
	man/Makefile 
	scripts/Makefile
	tests/Makefile
	tests/ghcaller/Makefile
	tests/diffseqs/Makefile
)


