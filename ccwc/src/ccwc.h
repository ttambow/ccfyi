#ifndef ccwc_h
#define ccwc_h

#define _POSIX_C_SOURCE 200809L // required for `fileno` (studio.h)

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <sys/stat.h>

#define help_text "no option(s) provided. usage: program_name [ -c | -h | -l | -m | -w ] <file>"
#define min_args 3

FILE *file;

//todo error_codes list

typedef enum { error = -1, bytes, lines, words, chars } commands;

int command, counter;

/* declarations */

int ccwc(char **argv);
commands check_flags(const char *flag);
void count(void);
void count_bytes(void);
void count_chars(void);
void count_lines(void);
void count_words(void);
char *get_file_mode(void);
void handle_error(int error_code);
void open_file(const char *filename);
void print_help_and_exit(void);

/* definitions */

inline int ccwc(char **argv)
{
	command = check_flags(argv[1]);

	if (command == error) return EXIT_FAILURE;

	char *filename = argv[2];

	open_file(filename);

	count();

	printf("%2d: %s\n", counter, filename);

	return fclose(file);
}

inline commands check_flags(const char *flag)
{
	if ( flag[0] != '-')			 return error;
	if ( strcmp( flag, "-c" ) == 0 ) return bytes;
	if ( strcmp( flag, "-h" ) == 0 ) print_help_and_exit();
	if ( strcmp( flag, "-l" ) == 0 ) return lines;
	if ( strcmp( flag, "-m" ) == 0 ) return chars;
	if ( strcmp( flag, "-w" ) == 0 ) return words;

	return error;
}

inline void count(void)
{
	switch (command)
	{
		case bytes:		count_bytes(); break;
		case lines:		count_lines(); break;
		case words:		count_words(); break;
		case chars:		count_chars(); break;
		default   :		handle_error(error); break;
	};
}

inline void count_bytes(void)
{
	unsigned char c;

	while (fread(&c, sizeof(char), 1, file) > 0) ++counter;
}

inline void count_chars(void)
{
	while (fgetwc(file) != WEOF) ++counter;
}

inline void count_lines(void)
{
	char c;

	while ((c = fgetc(file)) != EOF) if (c == '\n') ++counter;
}

inline void count_words(void)
{
	char c;
	bool is_char = false;

	while ( ( c = fgetc( file ) ) != EOF )
		if ( isspace( c ) ) is_char = false;
		else if ( !is_char ) is_char = true, ++counter;
}

inline void handle_error(const int error_code)
{
	errno = error_code;
	printf("error! code: %d\n", error_code);

	if (file != NULL)
		fclose(file);

	exit(error_code);
}

inline char *get_file_mode()
{
	switch (command)
	{
		case bytes:
		case chars:
			return "rb";
		case lines:
		case words:
			return "r";
		default:
			assert("get_file_mode is breached. a bug! a bug!");
			return NULL;
	}
}

inline void open_file(const char *filename)
{
	if (filename == NULL)	handle_error(EINVAL);

	file = fopen(filename, get_file_mode());

	if ( file == NULL )	handle_error(ENOENT);
	if ( ferror(file) )	handle_error(errno ? errno : EIO);

	struct stat st;

	if ( fstat( fileno(file), &st) != 0 )	handle_error(errno);
	if ( S_ISDIR( st.st_mode ) )				handle_error(EISDIR);

	if ( fseek(file, 0, SEEK_END) == 0)
		 rewind(file);
	else clearerr(file);
}

inline void print_help_and_exit(void)
{
	printf("%s\n", help_text);
	exit(EXIT_SUCCESS);
}

#endif // ccwc_h