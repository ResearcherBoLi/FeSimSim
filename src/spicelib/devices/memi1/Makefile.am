## Process this file with automake to produce Makefile.in

noinst_LTLIBRARIES = libmemi1.la

libmemi1_la_SOURCES =\
	memi1.c	\
	memi1defs.h	\
	memi1ask.c		\
	memi1param.c	\
	memi1mask.c	\
	memi1mpar.c	\
	memi1ext.h		\
	memi1init.c	\
	memi1init.h	\
	memi1itf.h	\
	memi1fbr.c	\
	memi1setup.c	\
	memi1load.c \
	memi1trunc.c \
	memi1conv.c \
	memi1getic.c



	

AM_CPPFLAGS = @AM_CPPFLAGS@ -I$(top_srcdir)/src/include
AM_CFLAGS = $(STATIC)

MAINTAINERCLEANFILES = Makefile.in
