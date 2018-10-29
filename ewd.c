/*	$OpenBSD: ntpd.c,v 1.106 2016/02/02 17:51:11 sthen Exp $ */

/*
 * Copyright (c) 2003, 2004 Henning Brauer <henning@openbsd.org>
 * Copyright (c) 2012 Mike Miller <mmiller@mgm51.com>
 * Copyright (c) 2017 Christopher Hettrick <chris@structfoo.com>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF MIND, USE, DATA OR PROFITS, WHETHER
 * IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING
 * OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <unistd.h>

#include "ewd.h"

#define NOTES	NOTE_DELETE|NOTE_WRITE|NOTE_TRUNCATE|NOTE_ATTRIB|NOTE_RENAME

__dead void	usage(void);
int		main(int, char *[]);

int
main(int argc, char *argv[])
{
	struct ewd_conf		 lconf;
	const char 		*conffile;
	int			 ch;
	struct rule		*r;
	int			 kq;
	struct kevent		 ke;
	int			 fd;

	conffile = CONFFILE;

	memset(&lconf, 0, sizeof(lconf));

	while ((ch = getopt(argc, argv, "df:nv")) != -1) {
		switch (ch) {
		case 'd':
			lconf.debug = 2;
			break;
		case 'f':
			conffile = optarg;
			break;
		case 'n':
			lconf.debug = 2;
			lconf.noaction = 1;
			break;
		case 'v':
			lconf.verbose++;
			break;
		default:
			usage();
			/* NOTREACHED */
		}
	}

	/* log to stderr until daemonized */
	log_init(lconf.debug ? lconf.debug : 1, LOG_DAEMON);

	argc -= optind;
	argv += optind;
	if (argc > 0)
		usage();

	if (parse_config(conffile, &lconf))
		exit(1);

	if (lconf.noaction) {
		fprintf(stderr, "configuration OK\n");
		exit(0);
	}

	if ((kq = kqueue()) < 0) {
		fatal("kqueue");
	}

	TAILQ_FOREACH(r, &conf->rules, entry) {
		printf("\nNext rule:\n");
		printf("The filename is: %s\n", r->filename);
		printf("The command is:  %s\n", r->command);

		if ((fd = open(r->filename, O_RDONLY)) == -1) {
			fprintf(stderr, "can't open: %s\n", r->filename);
		}

		EV_SET(&ke, fd, EVFILT_VNODE, EV_ADD|EV_CLEAR, NOTES, 0,
		    (void *)r);

		if (kevent(kq, &ke, 1, NULL, 0, NULL) == -1) {
			fatal("kevent");
		}
	}

	for (;;) {
		kevent(kq, NULL, 0, &ke, 1, NULL);
		system(((struct rule *)ke.udata)->command);
	}

	return (0);
}

__dead void
usage(void)
{
	extern char		*__progname;

	fprintf(stderr, "usage: %s [-dnv] [-f file]\n", __progname);
	exit(1);
}
