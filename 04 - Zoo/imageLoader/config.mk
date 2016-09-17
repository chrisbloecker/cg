abs_srcdir   := /home/chris/tmp/cg1/ueb04/imageLoader
abs_builddir := /home/chris/tmp/cg1/ueb04/imageLoader

# programs
SHELL    := /bin/bash
CPP      := gcc -E
CC       := gcc
AR       := ar

DEPEND      := :
NATURALDOCS := 

# generic flags
CFLAGS   := -g -O2
CPPFLAGS :=  -I$(srcdir)/include -I$(builddir)
LIBS     := -lz 

# optional libraries
HAVE_LIBZ   := yes

# install pathes
prefix      := /usr/local
exec_prefix := ${prefix}
bindir      := ${exec_prefix}/bin
sbindir     := ${exec_prefix}/sbin
libexecdir  := ${exec_prefix}/libexec
datadir     := ${prefix}/share
sysconfdir  := ${prefix}/etc
libdir      := ${exec_prefix}/lib
includedir  := ${prefix}/include
mandir      := ${prefix}/man
docdir      := ${datadir}/doc/image-loader
