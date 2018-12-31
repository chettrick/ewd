#include "test.h"

int	triple_me(int);

int
main(void)
{
	int some_num = -4;		/* Just some negative number. */

	plan(9);			/* Number of tests to run. */

	diag("Testing example.c");	/* Diagnostic information on tests. */

	ok(5 == 6, "five equals six");	/* Should fail. */
	ok(5 == 5, "five equals five");	/* Should pass. */

	skip(5 == 7, "5 equals 7");	/* Failing test is skipped entirely. */
	skip(7 != 8, "seven isn't 8");	/* Passing test is skipped entirely. */

	todo(5 == 59, "5 equals 59");	/* Test fails, but it is a TODO. */
	todo(4 + 4 == 8, "addition");	/* It passes. Now, turn it into 'ok'. */

	ok(triple_me(5) == 15, "triple_me(5) is 15");	/* Function calls. */
	ok(!triple_me(0), "triple_me(0) is 0");		/* Function calls. */

	ok(some_num, "number is non-zero");	/* Negative numbers are true. */

	return 0;
}

int
triple_me(int value)
{
	return (3 * value);
}
