## Process this file with automake to produce Makefile.in

bin_PROGRAMS = gspeechd

gspeechd_SOURCES = gspeechd.c \
	gspeechd-client-context.c \
	gspeechd-server.c \
	gspeechd-log.c \
	gspeechd-options.c

gspeechd_CFLAGS = -Wall
gspeechd_CPPFLAGS = $(GIO_CFLAGS) -I$(top_srcdir)/src
gspeechd_LDADD = $(GIO_LIBS) $(top_builddir)/src/ssip/libssip.la