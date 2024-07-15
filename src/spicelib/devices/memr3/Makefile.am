## Process this file with automake to produce Makefile.in

noinst_LTLIBRARIES = libmemr3.la

libmemr3_la_SOURCES =\
	memr3.c	\
	memr3defs.h	\
	memr3ask.c		\
	memr3param.c	\
	memr3mask.c	\
	memr3mpar.c	\
	memr3ext.h		\
	memr3init.c	\
	memr3init.h	\
	memr3itf.h	\
	memr3fbr.c	\
	memr3setup.c	\
	memr3load.c \
	memr3trunc.c \
	memr3conv.c \
	memr3getic.c



	

AM_CPPFLAGS = @AM_CPPFLAGS@ -I$(top_srcdir)/src/include
AM_CFLAGS = $(STATIC)

MAINTAINERCLEANFILES = Makefile.in
