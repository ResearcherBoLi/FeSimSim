## Process this file with automake to produce Makefile.in

noinst_LTLIBRARIES = libfecap1.la

libfecap1_la_SOURCES =\
	fecap1.c	\
	fecap1defs.h	\
	fecap1ask.c		\
	fecap1param.c	\
	fecap1mask.c	\
	fecap1mpar.c	\
	fecap1ext.h		\
	fecap1init.c	\
	fecap1init.h	\
	fecap1itf.h	\
	fecap1fbr.c  \
	fecap1setup.c \
	fecap1load.c \
	fecap1acld.c \
	fecap1trunc.c \
	fecap1conv.c \
	fecap1getic.c



	

AM_CPPFLAGS = @AM_CPPFLAGS@ -I$(top_srcdir)/src/include
AM_CFLAGS = $(STATIC)

MAINTAINERCLEANFILES = Makefile.in
