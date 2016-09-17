abs_srcdir   := /home/chris/svn/cg1/ueb05/imageLoader
abs_builddir := /home/chris/svn/cg1/ueb05/imageLoader

# programs
SHELL    := /bin/sh
CPP      := gcc -E
CC       := gcc
AR       := ar

DEPEND      := makedepend
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
