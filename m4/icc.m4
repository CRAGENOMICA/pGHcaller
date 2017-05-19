AC_DEFUN([AC_PROG_ICC],
[
    AC_ARG_WITH(icc,
            AC_HELP_STRING([--with-icc],[Code will be compiled with icc]),
    [ if test "$withval" != "yes"; then
	  icc="yes"
      else
	  icc="no"
      fi
     ])

     if test -z "$icc"; then
         AC_MSG_CHECKING([for icc])
         icc=`mpicxx --version | grep "^icc"`
	 if test -n "$icc"; then
	     icc="yes"
	 else
	     icc="no"
	 fi
         AC_MSG_RESULT([$icc])
     fi

     if test "yes" == "$icc"; then
	  USING_ICC="-DICC"
          ICC_NO_WARN_SOME="-wd981 -wd383"
     else
     	  USING_ICC=""
          ICC_NO_WARN_SOME=""
     fi

     AC_SUBST(USING_ICC)          
     AC_SUBST(ICC_NO_WARN_SOME)
])

