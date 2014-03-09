#
# CDDL HEADER START
#
# The contents of this file are subject to the terms of the
# Common Development and Distribution License (the "License").
# You may not use this file except in compliance with the License.
#
# You can obtain a copy of the license at usr/src/OPENSOLARIS.LICENSE
# or http://www.opensolaris.org/os/licensing.
# See the License for the specific language governing permissions
# and limitations under the License.
#
# When distributing Covered Code, include this CDDL HEADER in each
# file and include the License file at usr/src/OPENSOLARIS.LICENSE.
# If applicable, add the following below this CDDL HEADER, with the
# fields enclosed by brackets "[]" replaced with your own identifying
# information: Portions Copyright [yyyy] [name of copyright owner]
#
# CDDL HEADER END
#
#
# Copyright 2009 Sun Microsystems, Inc.  All rights reserved.
# Use is subject to license terms.
#

PROG = kstat
OBJS = kstat.o
SRCS =$(OBJS:%.o=%.c) $(COMMON_SRCS)

LIBCOMMON = common/libcommon.a
LIBCURL = ../curl/libtmcurl.a

#include $(SRC)/cmd/Makefile.cmd
#include $(SRC)/cmd/stat/Makefile.stat

LDLIBS += -lavl -lcmdutils -ldevinfo -lgen -lkstat
CFLAGS += $(CCVERBOSE) -I${STATCOMMONDIR} -Wall
CERRWARN += -_gcc=-Wno-uninitialized
CERRWARN += -_gcc=-Wno-switch
CERRWARN += -_gcc=-Wno-parentheses

CPPFLAGS_sparc += -I$(SRC)/uts/sfmmu
CPPFLAGS_sparc += -I$(SRC)/uts/sun4u/sunfire
CPPFLAGS += $(CPPFLAGS_$(MACH))

FILEMODE= 0555

lint := LINTFLAGS = -muxs -I$(STATCOMMONDIR)

.KEEP_STATE:

all: curl $(PROG)

curl:
	make -C ../curl

install: all $(ROOTPROG)

kstat: kstat.c $(LIBCOMMON) $(LIBCURL)
	cc -Wall -g -o kstat kstat.c -L./common -L../curl -lcommon -ltmcurl -lcurl -lkstat -ldevinfo -lcmdutils -lavl -lgen

$(LIBCOMMON): 
	cd common; make

$(LIBCURL):
	cd ../curl; make

clean:
	-$(RM) $(OBJS) $(COMMON_OBJS)
	-$(RM) kstat

lint: lint_SRCS

#include $(SRC)/cmd/Makefile.targ
