PROG=	example

SRCS=	example.c

NOMAN=

OBJS=	${SRCS:.c=.o}

all: test

test: ${PROG}
	prove -ve '' ${PROG}

${PROG}: ${OBJS}
	${CC} ${CFLAGS} ${LDFLAGS} -o $@ ${OBJS}

clean:
	rm -f a.out [Ee]rrs mklog *.core y.tab.h ${PROG} *.d *.o
