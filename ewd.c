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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "ewd.h"

__dead void	usage(void);
int		main(int, char *[]);

__dead void
usage(void)
{
	extern char		*__progname;

	fprintf(stderr, "usage: %s [-dnv] [-f file]\n", __progname);
	exit(1);
}

int
main(int argc, char *argv[])
{
	struct ewd_conf		 lconf;
	const char 		*conffile;
	int			 ch;

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

	return (0);
}
