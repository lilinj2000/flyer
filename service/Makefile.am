AUTOMAKE_OPTIONS = foreign

lib_LTLIBRARIES = libflyer.la
libflyer_la_SOURCES = \
		TraderOptions.cc TraderSpiImpl.cc \
		TraderServiceImpl.cc

libflyer_la_CPPFLAGS = -I$(top_srcdir) \
			$(BOOST_CPPFLAGS) \
			$(SGI_CPPFLAGS) \
			$(SOIL_CPPFLAGS)

libflyer_la_LIBADD = $(SGI_LIBADD) \
		$(BOOST_LIBADD) \
		$(SOIL_LIBADD)

libflyer_la_LDFLAGS = \
	-no-undefined -version-info \
	$(LT_CURRENT):$(LT_REVISION):$(LT_AGE) \
	-release $(LT_RELEASE)

# header files
noinst_HEADERS = \
	TraderOptions.hh \
	TraderServiceImpl.hh \
	TraderSpiImpl.hh \
	Log.hh \
	SgitFtdcUserApiStructPrint.hh


