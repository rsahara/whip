SUBDIRS = \
    Document \
    PseudoCode \
    Util \
    Formats

noinst_LTLIBRARIES = libWhip.la

libWhip_la_CPPFLAGS = $(AM_CPPFLAGS)
libWhip_la_CXXFLAGS = $(AM_CXXFLAGS)

libWhip_la_LIBADD = \
    $(builddir)/Document/libDocument.la \
    $(builddir)/PseudoCode/libPseudoCode.la \
    $(builddir)/Util/libUtil.la \
    $(builddir)/Formats/libFormats.la

libWhip_la_SOURCES = \
    SupportedFormats.cpp

noinst_HEADERS = \
    Whip.hpp \
    SupportedFormats.hpp
