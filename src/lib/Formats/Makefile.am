SUBDIRS = \
    DotGraphFormat \
    SwiftFormat \
    KotlinFormat \
    SmFormat    

noinst_LTLIBRARIES = libFormats.la

libFormats_la_CPPFLAGS = $(AM_CPPFLAGS)
libFormats_la_CXXFLAGS = $(AM_CXXFLAGS)

libFormats_la_LIBADD = \
    $(builddir)/DotGraphFormat/libDotGraphFormat.la \
    $(builddir)/SwiftFormat/libSwiftFormat.la \
    $(builddir)/KotlinFormat/libKotlinFormat.la \
    $(builddir)/SmFormat/libSmFormat.la

libFormats_la_SOURCES = \
    DotGraphDocumentWriter.cpp \
    SwiftDocumentWriter.cpp \
    SwiftEnumDocumentWriter.cpp \
    KotlinDocumentWriter.cpp \
    SmDocumentReader.cpp

noinst_HEADERS = \
    DotGraphDocumentWriter.hpp \
    SwiftDocumentWriter.hpp \
    SwiftEnumDocumentWriter.hpp \
    KotlinDocumentWriter.hpp \
    SmDocumentReader.hpp
