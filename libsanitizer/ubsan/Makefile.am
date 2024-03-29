AM_CPPFLAGS = -I $(top_srcdir) -I $(top_srcdir)/include

# May be used by toolexeclibdir.
gcc_version := $(shell cat $(top_srcdir)/../gcc/BASE-VER)

DEFS = -D_GNU_SOURCE -D_DEBUG -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS 
AM_CXXFLAGS = -Wall -W -Wno-unused-parameter -Wwrite-strings -pedantic -Wno-long-long  -fPIC -fno-builtin -fno-exceptions -fno-rtti -fomit-frame-pointer -funwind-tables -fvisibility=hidden -Wno-variadic-macros
AM_CXXFLAGS += $(LIBSTDCXX_RAW_CXX_CXXFLAGS)
ACLOCAL_AMFLAGS = -I m4

toolexeclib_LTLIBRARIES = libubsan.la

ubsan_files = \
	ubsan_diag.cc \
	ubsan_flags.cc \
	ubsan_handlers.cc \
	ubsan_handlers_cxx.cc \
	ubsan_init.cc \
	ubsan_type_hash.cc \
	ubsan_value.cc

libubsan_la_SOURCES = $(ubsan_files) 
libubsan_la_LIBADD = $(top_builddir)/sanitizer_common/libsanitizer_common.la 
if !USING_MAC_INTERPOSE
libubsan_la_LIBADD += $(top_builddir)/interception/libinterception.la
endif
if LIBBACKTRACE_SUPPORTED
libubsan_la_LIBADD += $(top_builddir)/libbacktrace/libsanitizer_libbacktrace.la
endif
libubsan_la_LIBADD += $(LIBSTDCXX_RAW_CXX_LDFLAGS)
libubsan_la_LDFLAGS = -version-info `grep -v '^\#' $(srcdir)/libtool-version` $(link_libubsan)

# Use special rules for files that require RTTI support.
ubsan_handlers_cxx.% ubsan_type_hash.% : AM_CXXFLAGS += -frtti

# Work around what appears to be a GNU make bug handling MAKEFLAGS
# values defined in terms of make variables, as is the case for CC and
# friends when we are called from the top level Makefile.
AM_MAKEFLAGS = \
	"AR_FLAGS=$(AR_FLAGS)" \
	"CC_FOR_BUILD=$(CC_FOR_BUILD)" \
	"CFLAGS=$(CFLAGS)" \
	"CXXFLAGS=$(CXXFLAGS)" \
	"CFLAGS_FOR_BUILD=$(CFLAGS_FOR_BUILD)" \
	"CFLAGS_FOR_TARGET=$(CFLAGS_FOR_TARGET)" \
	"INSTALL=$(INSTALL)" \
	"INSTALL_DATA=$(INSTALL_DATA)" \
	"INSTALL_PROGRAM=$(INSTALL_PROGRAM)" \
	"INSTALL_SCRIPT=$(INSTALL_SCRIPT)" \
	"JC1FLAGS=$(JC1FLAGS)" \
	"LDFLAGS=$(LDFLAGS)" \
	"LIBCFLAGS=$(LIBCFLAGS)" \
	"LIBCFLAGS_FOR_TARGET=$(LIBCFLAGS_FOR_TARGET)" \
	"MAKE=$(MAKE)" \
	"MAKEINFO=$(MAKEINFO) $(MAKEINFOFLAGS)" \
	"PICFLAG=$(PICFLAG)" \
	"PICFLAG_FOR_TARGET=$(PICFLAG_FOR_TARGET)" \
	"SHELL=$(SHELL)" \
	"RUNTESTFLAGS=$(RUNTESTFLAGS)" \
	"exec_prefix=$(exec_prefix)" \
	"infodir=$(infodir)" \
	"libdir=$(libdir)" \
	"prefix=$(prefix)" \
	"includedir=$(includedir)" \
	"AR=$(AR)" \
	"AS=$(AS)" \
	"LD=$(LD)" \
	"LIBCFLAGS=$(LIBCFLAGS)" \
	"NM=$(NM)" \
	"PICFLAG=$(PICFLAG)" \
	"RANLIB=$(RANLIB)" \
	"DESTDIR=$(DESTDIR)"

MAKEOVERRIDES=

## ################################################################

