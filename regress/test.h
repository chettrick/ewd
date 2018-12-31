/*
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
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

/**
 * Trivial hand-rolled C testing framework.
 *
 * This testing framework conforms to TAP12 (Test Anything Protocol ver 12).
 * Output can be programmatically evaluated using the 'prove' command,
 * which is part of the Perl module Test::Harness, or other such utilities.
 *      E.g. $ prove -ve '' program_name
 *
 * - Every module test needs one plan(), and then subsequent tests via ok().
 * - Use todo() for tests that are made before code is written (think TDD).
 *     Convert them over to ok() once the new code is in a testable state.
 * - Use skip() for not running a test at all. skip() always outputs 'ok'
 *     whether or not the test would actually pass.
 * - Use diag() for diagnostic (informational) messages in test output.
 *
 * The whole testing framework is implemented with preprocessor macros.
 *
 * #include "test.h" in each source file that needs it.
 *
 * A single, independent source file should be used to organize all tests,
 *   either on a module-wide basis, or a project-wide basis.
 */

#ifndef _TEST_H_
#define _TEST_H_

#include <stdio.h>

int	test_count;		/* Number of actual tests performed. */

/**
 * Supply the test plan with the number of tests to run.
 *
 * plan() should be the first macro called, with total number of tests.
 * If it is impossibe to know in advance the number of tests to run,
 * then plan(test_count) as the last macro called is also correct,
 * but not preferrable nor advised.
 *
 * @param total_tests Total number of tests to run.
 */
#define									\
plan(total_tests)							\
{									\
	printf("1..%d\n", (total_tests));				\
}

/**
 * Perform the supplied test and output whether it passed or not.
 * A non-zero value for test is a pass. A zero value for test is a fail.
 *
 * @param test A boolean expression to test for truth.
 * @param msg A string containing a short description of the test.
 *
 * Note: This macro outputs:	[not ok|ok] <test_num> - <msg>
 *       and on test failure:		# Test failed in <file> at line <num>
 */
#define									\
ok(test, msg)								\
{									\
	if ((test)) {							\
		printf("ok %d - %s\n", ++test_count, (msg));		\
	} else {							\
		printf("not ok %d - %s\n", ++test_count, (msg));	\
		printf("\t# Test failed in %s at line %d\n",		\
		    __FILE__, __LINE__);				\
	}								\
}

/**
 * Mark the supplied test as a TODO, but still run the test.
 *
 * @param test A boolean expression to test for truth.
 * @param msg A string containing a short description of the test.
 *
 * Note: This macro outputs:	[not ok|ok] <test_num> - <msg> # TODO
 *       and on test failure:		# Test failed in <file> at line <num>
 */
#define									\
todo(test, msg)								\
{									\
	if ((test)) {							\
		printf("ok %d - %s # TODO\n", ++test_count, (msg));	\
	} else {							\
		printf("not ok %d - %s # TODO\n", ++test_count, (msg));	\
		printf("\t# Test failed in %s at line %d\n",		\
		    __FILE__, __LINE__);				\
	}								\
}

/**
 * Mark the supplied test as a SKIP, and do not run the test.
 *
 * @param test A boolean expression to test for truth.
 * @param msg A string containing a short description of the test.
 *
 * Note: This macro always outputs: 'ok <test_num> <msg> # SKIP
 */
#define									\
skip(test, msg)								\
{									\
	printf("ok %d - %s # SKIP\n", ++test_count, (msg));		\
}

/**
 * Print a diagnostic message in the testing output.
 *
 * @param msg A string containing a short diagnostic message.
 *
 * Note: This macro outputs: # <msg>
 */
#define									\
diag(msg)								\
{									\
	printf("# %s\n", (msg));					\
}

#endif /* _TEST_H_ */
