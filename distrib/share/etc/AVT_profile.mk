CFLAGS = -g -pg -Wall -W ${OSCFLAGS} -DDELAY_DEBUG_STAT -DAVT_MORE_INFOS
CPPFLAGS = 
STRIP = $(ECHO)
ifndef AVERTEC_LICENSE
AVERTEC_LICENSE = NO_LICENSE
endif

