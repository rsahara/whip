ACLOCAL_AMFLAGS = -I m4
SUBDIRS = src test

bin_PROGRAMS = whip
whip_CPPFLAGS = -I$(top_srcdir)/src/app $(AM_CPPFLAGS)
whip_CXXFLAGS = $(AM_CXXFLAGS)
whip_SOURCES = $(top_srcdir)/src/main.cpp
whip_LDADD = \
    $(top_builddir)/src/app/libCommand.la \
    $(top_builddir)/src/lib/libWhip.la

EXTRA_DIST = README.md
