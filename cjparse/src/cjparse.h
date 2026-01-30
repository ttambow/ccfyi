#ifndef cjparse_h
#define cjparse_h

#define _POSIX_C_SOURCE 200809L // required for `fileno` (studio.h)

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <wchar.h>

#define help_text "no option(s) provided. usage: program_name <file>"

enum { error = -1, bytes, lines, words, chars };

/* declarations below */

void print_help(void);

/* declarations above */
/* definitions below */

inline void print_help(void)
{
	printf("%s\n", help_text);
	exit(EXIT_SUCCESS);
}

/* definitions above */

#endif // cjparse_h