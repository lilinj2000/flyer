AUTOMAKE_OPTIONS = foreign

SUBDIRS = traderflow

TESTS = unittest.sh

check_PROGRAMS = unittest

unittest_SOURCES = \
	TraderServiceImplTest.cc


unittest_CPPFLAGS = -I$(top_srcdir) -I.. -I..\
		$(BOOST_CPPFLAGS) \
		$(ZEUSING_CPPFLAGS) \
		$(GTEST_CPPFLAGS) \
		$(SOIL_CPPFLAGS)


unittest_LDADD = \
	$(GTEST_LIBADD) \
	../service/libzero.la \
	$(BOOST_LIBADD) \
	$(ZEUSING_LIBADD) \
	$(SOIL_LIBADD)



unittest_LDFLAGS = $(LIBADD_DL) -Wl,--no-as-needed -pthread

# header files
noinst_HEADERS = 

EXTRA_DIST = env.sh zero.cfg log.cfg

