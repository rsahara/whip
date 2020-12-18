AC_INIT([whip], [1.0], [contact@runo.jp])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_SRCDIR([src/main.cpp])
AC_CONFIG_AUX_DIR([aux-config])
AC_CONFIG_SRCDIR([configure.h.in])
AC_CONFIG_HEADERS([configure.h])
AM_INIT_AUTOMAKE([subdir-objects foreign])
AC_PROG_CXX
LT_INIT
AX_CXX_COMPILE_STDCXX_17([noext],[mandatory])
AC_SUBST([AM_CXXFLAGS], ['-Wall'])
AC_SUBST([AM_CPPFLAGS], ['-I$(top_srcdir)/src/lib'])

AC_CONFIG_FILES([
	Makefile
	src/Makefile
	src/lib/Makefile
	src/lib/Util/Makefile
	src/lib/Document/Makefile
	src/lib/PseudoCode/Makefile
	src/lib/Formats/Makefile
	src/lib/Formats/DotGraphFormat/Makefile
	src/lib/Formats/SmFormat/Makefile
	src/lib/Formats/SwiftFormat/Makefile
	src/lib/Formats/KotlinFormat/Makefile
	src/app/Makefile
	test/Makefile
])
AC_OUTPUT
