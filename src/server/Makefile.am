## Process this file with automake to produce Makefile.in

bin_PROGRAMS = gspeechd

gspeechd_SOURCES = gspeechd.c \
	gspeechd-client.c \
	gspeechd-server.c \
	gspeechd-log.c \
	gspeechd-options.c

gspeechd_CFLAGS = -Wall
gspeechd_CPPFLAGS = $(GIO_CFLAGS) $(GIO_UNIX_CFLAGS) -I$(top_srcdir)/src
gspeechd_LDADD = $(GIO_LIBS) $(GIO_UNIX_LIBS) $(top_builddir)/src/ssip/libssip.la