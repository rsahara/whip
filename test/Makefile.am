check_PROGRAMS = whip tester

whip_CPPFLAGS = -I$(top_srcdir)/src/app $(AM_CPPFLAGS)
whip_CXXFLAGS = $(AM_CXXFLAGS)
whip_SOURCES = $(top_srcdir)/src/main.cpp
whip_LDADD = $(top_builddir)/src/app/libCommand.la $(top_builddir)/src/lib/libWhip.la

tester_CPPFLAGS = -I$(top_srcdir)/test/src $(AM_CPPFLAGS)
tester_CXXFLAGS = $(AM_CXXFLAGS)
tester_SOURCES = src/tester.cpp \
    src/tester/Text.cpp
noinst_HEADERS = \
    src/tester/Text.hpp
tester_LDADD = $(top_builddir)/src/lib/libWhip.la

AM_TESTS_ENVIRONMENT = \
    PROGRAM_PATH='$(abs_builddir)'/whip; \
    export PROGRAM_PATH; \
    TESTER_PATH='$(abs_builddir)'/tester; \
    export TESTER_PATH; \
    TEST_PATH='$(abs_srcdir)'; \
    export TEST_PATH;

TEST_EXTENSIONS = .sh
SH_LOG_COMPILER = $(SHELL)
AM_SH_LOG_FLAGS = -vex

INPUT_FILES = \
    files/valid-exhaustive.sm \
    files/valid-sample.sm \
    files/swift/valid-sample/Package.swift \
    files/swift/valid-sample/Sources \
    files/swift/valid-sample.expected-out

SCRIPT_FILES = \
    test-usage.sh \
    test-valid-input.sh \
    test-text-features.sh \
    test-valid-sample-swift.sh

EXTRA_DIST = $(INPUT_FILES) $(SCRIPT_FILES)
TESTS = $(SCRIPT_FILES)
