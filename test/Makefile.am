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

TESTDATA = \
    files/valid-exhaustive.sm \
    files/valid-sample.sm

TESTSCRIPTS = \
    test-usage.sh \
    test-valid-input.sh \
    test-text-features.sh

EXTRA_DIST = $(TESTDATA) $(TESTSCRIPTS)
TESTS = $(TESTSCRIPTS)
