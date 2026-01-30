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

typedef enum { error = -1, _help, bytes, lines, words, chars } commands;

int command = error /* default */, counter = 0;

int ccwc(char **argv);					// library's entry
void check_flags(const char *flag);		// determines which counter function to use
void count_bytes(void);					// counts the number of bytes in a given file
void count_chars(void);					// counts the number of characters in a given file
void count_handler(void);				// coordinates which counter function to use
void count_lines(void);					// counts the number of new lines in a given file
void count_words(void);					// counts the number of words in a given file
char *get_file_mode(void);				// switch to determine file mode
void handle_error(int error_code);		// calls exit()
void open_file(const char *file_name);	// may call exit()
void print_help(void);					// calls exit()

inline int ccwc(char **argv)
{
	check_flags(argv[1]);
	char *filename = argv[2];
	open_file(filename);
	count_handler();
	printf("%2d: %s\n", counter, filename);

	return fclose(file);
}

inline void check_flags(const char *flag)
{
	if ( strcmp( flag, "-c" ) == 0 )	command = bytes;
	if ( strcmp( flag, "-h" ) == 0 )	command = _help;
	if ( strcmp( flag, "-l" ) == 0 )	command = lines;
	if ( strcmp( flag, "-m" ) == 0 )	command = chars;
	if ( strcmp( flag, "-w" ) == 0 )	command = words;
	if ( flag[0] != '-')				command = error;

	if (command == _help)				print_help();
	if (command == error)				exit(EXIT_FAILURE);
}

inline void count_bytes(void)
{
	fseek(file, 0, SEEK_END);
	counter = ftell(file);
}

inline void count_chars(void)
{
	unsigned char c;
	while (fread(&c, 1, 1, file) == 1)
		// utf "continuation" bytes are not valid chars
		if ((c & 0xC0) != 0x80) ++counter;
}

inline void count_handler(void)
{
	switch (command)
	{
		case bytes:		count_bytes(); break;
		case lines:		count_lines(); break;
		case words:		count_words(); break;
		case chars:		count_chars(); break;
		default   :		handle_error(error); break;
	}
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

inline char *get_file_mode(void)
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

inline void handle_error(const int error_code)
{
	errno = error_code;
	printf("error! code: %d\n", error_code);

	if (file != NULL)
		fclose(file);

	exit(error_code);
}

inline void open_file(const char *file_name)
{
	if (file_name == NULL) handle_error(EINVAL);

	file = fopen(file_name, get_file_mode());

	if ( file == NULL )	handle_error(ENOENT);
	if ( ferror(file) )	handle_error(errno);

	struct stat st;

	if ( fstat( fileno(file), &st) != 0 ) handle_error(errno);
	if ( S_ISDIR( st.st_mode ) ) handle_error(EISDIR);
	if ( fseek(file, 0, SEEK_END) == 0) rewind(file);
	else {
		clearerr(file);
		exit(EXIT_FAILURE);
	}
}

inline void print_help(void)
{
	printf("%s\n", help_text);
	exit(EXIT_SUCCESS);
}

#endif // ccwc_h