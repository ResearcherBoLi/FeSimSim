## Process this file with automake to produce Makefile.in

noinst_LTLIBRARIES = libmemr2.la

libmemr2_la_SOURCES =\
	memr2.c	\
	memr2defs.h	\
	memr2ask.c		\
	memr2param.c	\
	memr2mask.c	\
	memr2mpar.c	\
	memr2ext.h		\
	memr2init.c	\
	memr2init.h	\
	memr2itf.h	\
	memr2fbr.c  \
	memr2setup.c \
	memr2load.c \
	memr2acld.c \
	memr2trunc.c \
	memr2conv.c \
    memr2noi.c \
    memr2acct.c \
	memr2getic.c



	

AM_CPPFLAGS = @AM_CPPFLAGS@ -I$(top_srcdir)/src/include
AM_CFLAGS = $(STATIC)

MAINTAINERCLEANFILES = Makefile.in
