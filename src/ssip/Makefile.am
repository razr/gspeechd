noinst_LTLIBRARIES = libssip.la
libssip_la_CFLAGS = $(GLIB_CFLAGS)
libssip_la_LIBADD = $(GLIB_LIBS)
libssip_la_SOURCES = \
	ssip-enum-types.h \
	ssip-enum-types.c \
	ssip-status-enum-types.h \
	ssip-status-enum-types.c \
	ssip.c

noinst_HEADERS = \
	ssip-enums.h

ssip-enum-types.h: ssip-enum-types.h.template
	$(AM_V_GEN) (cd $(srcdir) && $(GLIB_MKENUMS) --template ssip-enum-types.h.template ssip-enums.h) > $@

ssip-enum-types.c: ssip-enum-types.c.template ssip-enum-types.h
	$(AM_V_GEN) (cd $(srcdir) && $(GLIB_MKENUMS) --template ssip-enum-types.c.template ssip-enums.h) > $@
	$(SED) -i "/^[ ]*{/s/-/_/g" $@

ssip-status-enum-types.h: ssip-status-enum-types.h.template
	$(AM_V_GEN) (cd $(srcdir) && $(GLIB_MKENUMS) --template ssip-status-enum-types.h.template ssip-status-enums.h) > $@

ssip-status-enum-types.c: ssip-status-enum-types.c.template ssip-status-enum-types.h
	$(AM_V_GEN) (cd $(srcdir) && $(GLIB_MKENUMS) --template ssip-status-enum-types.c.template ssip-status-enums.h) > $@
	$(SED) -i "/^[ ]*{/s/-/ /g" $@

CLEANFILES =	\
	ssip-enum-types.c	\
	ssip-enum-types.h	\
	ssip-status-enum-types.c	\
	ssip-status-enum-types.h

EXTRA_DIST = \
	ssip-enum-types.c.template \
	ssip-enum-types.h.template	\
	ssip-status-enum-types.c.template	\
	ssip-status-enum-types.h.template
