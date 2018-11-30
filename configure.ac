AC_PREREQ([2.69])

AC_INIT(hpl, 2.2, hpl@icl.utk.edu)
AC_CONFIG_SRCDIR([include/hpl.h])
AC_CONFIG_HEADERS([include/hplconfig.h])

AC_PROG_CC

AC_PROG_RANLIB

AC_PROG_INSTALL

AM_INIT_AUTOMAKE([subdir-objects])

AM_PROG_CC_C_O

AC_CHECK_HEADERS([mpi.h])

AC_CHECK_FUNCS([dgemm_])

AC_CONFIG_FILES([Makefile src/Makefile testing/Makefile])
AC_OUTPUT
