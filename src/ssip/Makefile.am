noinst_LTLIBRARIES = libssip.la
libssip_la_CFLAGS = $(GLIB_CFLAGS)
libssip_la_LIBADD = $(GLIB_LIBS)
libssip_la_SOURCES = \
	ssip.c \
	ssip-enum-types.c	\
	ssip-enum-types.h

noinst_HEADERS = \
	ssip-enums.h

ssip-enum-types.h: ssip-enum-types.h.template $(NOINST_H_FILES)
	$(AM_V_GEN) (cd $(srcdir) && $(GLIB_MKENUMS) --template ssip-enum-types.h.template $(noinst_HEADERS)) > $@

ssip-enum-types.c: ssip-enum-types.c.template $(NOINST_H_FILES)
	$(AM_V_GEN) (cd $(srcdir) && $(GLIB_MKENUMS) --template ssip-enum-types.c.template $(noinst_HEADERS)) > $@

CLEANFILES =	\
	ssip-enum-types.c	\
	ssip-enum-types.h

EXTRA_DIST = \
	ssip-enum-types.c.template \
	ssip-enum-types.h.template
