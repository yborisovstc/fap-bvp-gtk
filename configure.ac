
AC_INIT([fap-bvp-gtk], [0.1], [yuri.borisov.v@gmail.com])

AM_INIT_AUTOMAKE([-Wall])

AC_CONFIG_FILES([
		 Makefile
		 src/Makefile
		 ])


AC_PROG_CC
AC_PROG_CXX
AC_PROG_LIBTOOL

# Configure options: --enable-debug[=no].
AC_ARG_ENABLE([debug],
	      [AS_HELP_STRING([--enable-debug],
			      [enable debug code (default is no)])],
			      [debug="$withval"], [debug=no])

PKG_CHECK_MODULES(GLIB, [glib-2.0 >= 2.0.0], , [AC_MSG_ERROR([
You must have the GLib 2.0 and development headers installed to build.
])])

PKG_CHECK_MODULES(LIBXML, [libxml-2.0], , [AC_MSG_ERROR([
You must have the libxml-2.0 and development headers installed to build.
])])

AC_SUBST(LIBXML_CFLAGS)
AC_SUBST(LIBXML_LIBS)


# TODO [YB] For Gtk view, to be removed after view separating
PKG_CHECK_MODULES(GTKPLUS, gtk+-2.0, , [
    AC_MSG_ERROR([You must have 'gtk+-2.0' dev headers (gtk+-2.0-dev) installed
    ]) ])

AC_SUBST(GTKPLUS_CFLAGS)
AC_SUBST(GTKPLUS_LIBS)


PKG_CHECK_MODULES(FAPWS, libfapws, , [
    AC_MSG_ERROR([You must have 'libfapws' dev headers (libfapws-dev) installed
    ]) ])

AC_SUBST(FAPWS_CFLAGS)
AC_SUBST(FAPWS_LIBS)



if test x$debug = xyes; then
    CXXFLAGS="-g -O0"
else
#    CXXFLAGS="-g -O2"
    CXXFLAGS="-g -O0"
fi


CXXFLAGS="$CXXFLAGS $GLIB_CFLAGS $LIBXML_CFLAGS $GTKPLUS_CFLAGS $FAPWS_CFLAGS" 
LDFLAGS="$LDFLAGS $GLIB_LIBS $LIBXML_LIBS $GTKPLUS_LIBS $FAPWS_LIBS"

AC_CONFIG_FILES([libfapbvpgtk.pc])

AC_OUTPUT

echo CXXFLAGS $CXXFLAGS
echo "
($PACKAGE_NAME) version $PACKAGE_VERSION
Prefix.........: $prefix
Debug Build....: $debug
C++ Compiler...: $CXX $CXXFLAGS $CPPFLAGS
Linker.........: $LD $LDFLAGS $LIBS
Doxygen........: ${DOXYGEN:-NONE}
"

