#	$OpenBSD: Makefile,v 1.16 2015/11/20 18:53:42 tedu Exp $

PREFIX?=/usr/local
BINDIR?=${PREFIX}/bin
MANDIR?=${PREFIX}/man/man

PROG=	ewd
SRCS=	ewd.c log.c parse.y

CFLAGS+= -Wall -I${.CURDIR}
CFLAGS+= -fstack-protector-all
CFLAGS+= -Wstrict-prototypes -Wmissing-prototypes
CFLAGS+= -Wmissing-declarations
CFLAGS+= -Wshadow -Wpointer-arith -Wcast-qual
CFLAGS+= -Wsign-compare

YFLAGS=

#LINKS=	${BINDIR}/ewd ${BINDIR}/ew

NOMAN=	ewd.8 ewd.conf.5 ew.8

OBJS=	${SRCS:.y=.c:.c=.o}

all: ${PROG}

${PROG}: ${OBJS}
	${CC} ${CFLAGS} ${LDFLAGS} -o $@ ${OBJS}

clean:
	rm -f a.out [Ee]rrs mklog *.core y.tab.h ${PROG} *.o *.d

.PHONY: all clean
