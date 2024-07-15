## Process this file with automake to produce Makefile.in

noinst_LTLIBRARIES = libmemr1.la

libmemr1_la_SOURCES =\
	memr1.c	\
	memr1defs.h	\
	memr1ask.c	\
	memr1param.c \
	memr1mask.c	\
	memr1mpar.c	\
	memr1ext.h		\
	memr1init.c	\
	memr1init.h	\
	memr1itf.h	\
	memr1fbr.c	\
	memr1setup.c	\
	memr1load.c \
	memr1acld.c \
	memr1trunc.c \
	memr1conv.c \
    memr1noi.c \
	memr1getic.c



	

AM_CPPFLAGS = @AM_CPPFLAGS@ -I$(top_srcdir)/src/include
AM_CFLAGS = $(STATIC)

MAINTAINERCLEANFILES = Makefile.in
