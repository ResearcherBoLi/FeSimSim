## Process this file with automake to produce Makefile.in

noinst_LTLIBRARIES = libmemc1.la

libmemc1_la_SOURCES =\
	memc1.c	\
	memc1defs.h	\
	memc1ask.c		\
	memc1param.c	\
	memc1mask.c	\
	memc1mpar.c	\
	memc1ext.h		\
	memc1init.c	\
	memc1init.h	\
	memc1itf.h	\
	memc1fbr.c	\
	memc1setup.c	\
	memc1load.c \
	memc1trunc.c \
	memc1conv.c \
	memc1getic.c



	

AM_CPPFLAGS = @AM_CPPFLAGS@ -I$(top_srcdir)/src/include
AM_CFLAGS = $(STATIC)

MAINTAINERCLEANFILES = Makefile.in
