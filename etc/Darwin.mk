# -*- Mode: Makefile -*-
#
####---------------------------------------------------------###
# description	: Alliance include file for Makefiles
# architecture	: SunOS londres 5.6 Generic_105181-16 sun4u sparc SUNW,Ultra-5_10
# date		: lundi,  4 octobre 1999, 10:16:59 WET DST
# file		: Solaris.mk
#

# The variables $ALLIANCE_* are set by
# alc_env.[c]sh script or libraries.mk

PROGRAM_SUFFIX=

GNU_LIB          =
GNU_INCLUDE      =

X11_LIB          = /usr/X11R6/lib/
X11_INCLUDE      = /usr/X11R6/include

MOTIF_LIB        = $(shell brew --prefix openmotif)/lib
MOTIF_INCLUDE    = $(shell brew --prefix openmotif)/include

XPM_LIB          = /usr/X11R6/lib
XPM_INCLUDE      = /usr/X11R6/include

SHELL		= /bin/sh
CSH		= /bin/csh
CP		= /bin/cp
CAT		= /bin/cat
MV		= /bin/mv
RM		= /bin/rm
MKDIR		= /bin/mkdir
FIND		= /bin/find
SED		= /usr/bin/env sed
AWK		= /usr/bin/env gawk
TR		= /usr/bin/env tr
TOUCH		= /usr/bin/touch
ECHO            = /bin/echo
STRIP		= /usr/bin/env strip
RANLIB		= /usr/bin/env ranlib

MAKE             = /usr/bin/make
MAKEFLAGS        = 

CC               = /usr/bin/gcc -fno-common
SCC              = $(CC)
CPLUSPLUS        = /usr/bin/g++
CPPFLAGS         ?= 

OPTIM            = -O2 -Wall

PURIFY           = purify

YACC             = $(shell brew --prefix bison)/bin/bison
YACCFLAGS        =
YACC_L           = -L$(shell brew --prefix bison)/lib

LEX              = $(shell brew --prefix flex)/bin/flex
LEX_H            = -I$(shell brew --prefix flex)/include
LEX_L            = -L$(shell brew --prefix flex)/lib

AR               = /usr/bin/env ar
ARFLAGS          = rv

SWIG             = $(shell brew --prefix swig)/bin/swig

TCL_H            = -I$(shell brew --prefix tcl-tk)/include/tcl-tk  -DHAVE_UNISTD_H
TCL_L            = -L$(shell brew --prefix tcl-tk)/lib -ltcl8.6

WHOLE            = -all_load -Wl,-no_warn_duplicate_libraries
NOWHOLE          = -Wl,-no_warn_duplicate_libraries

CFLAGS           = $(TCL_H) $(LEX_H) -O3 -Wno-knr-promoted-parameter
# EOF
