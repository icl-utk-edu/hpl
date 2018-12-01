
AC_DEFUN([HPL_BLAS], [

AC_PREREQ(2.69)

hpl_blas_ok=no

dnl FIXME: add --with-blas="<library spec>"

current_LIBS="$LIBS"

if test  x$hpl_blas_ok = xno; then
dnl
dnl -Wl,--start-group -lmkl_intel_lp64 -lmkl_sequential -lmkl_core -Wl,--end-group -lpthread
dnl
LIBS="-lmkl_intel_lp64 -lmkl_sequential -lmkl_core -lpthread $LIBS"
AC_MSG_CHECKING([for dgemm_ in sequential Intel MKL LP64])
AC_TRY_LINK_FUNC(dgemm_, [hpl_blas_ok=yes;BLAS_LIBS="-lmkl_intel_lp64 -lmkl_sequential -lmkl_core -lpthread"])
AC_MSG_RESULT($hpl_blas_ok)
LIBS="$current_LIBS"
dnl
fi

if test  x$hpl_blas_ok = xno; then
dnl
LIBS="-framework Accelerate $LIBS"
AC_MSG_CHECKING([for dgemm_ in -framework Accelerate])
AC_TRY_LINK_FUNC(dgemm_, [hpl_blas_ok=yes;BLAS_LIBS="-framework Accelerate"])
AC_MSG_RESULT($hpl_blas_ok)
LIBS="$current_LIBS"
dnl
fi

if test  x$hpl_blas_ok = xno; then
dnl
AC_MSG_CHECKING([for dgemm_ in OpenBLAS])
AC_CHECK_LIB(openblas, dgemm_, [hpl_blas_ok=yes;BLAS_LIBS="-lopenblas"])
AC_MSG_RESULT($hpl_blas_ok)
dnl
fi

dnl FIXME: test for Apple's vecLib
dnl FIXME: test for ATLAS
dnl FIXME: test for Netlib BLAS
dnl FIXME: test for ESSL
dnl FIXME: test for nvblas
dnl FIXME: test for ACML
dnl FIXME: test for GSL CBLAS

AC_SUBST(BLAS_LIBS)

# If present, execute ACTION-IF-FOUND/ACTION-IF-NOT-FOUND:
if test x"$hpl_blas_ok" = xyes; then
        ifelse([$1],,AC_DEFINE(HAVE_BLAS,1,[Define if you have a BLAS library.]),[$1])
        :
else
        hpl_blas_ok=no
        $2
fi

])dnl HPL_BLAS
