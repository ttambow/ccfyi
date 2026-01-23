#ifndef ccwc_h
#define ccwc_h

#define _POSIX_C_SOURCE 200809L // required for `fileno` (studio.h)

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <sys/stat.h>

#define help_text "no option(s) provided. usage: program_name [ -c | -h | -l | -m | -w ] <file>"
#define min_args 3

enum { error = -1, bytes, lines, words, chars };

/* declarations */

int ccwc(int argc, char **argv);
int counter(int command, const char *file);
int count_bytes(const char *filename);
int count_chars(const char *filename);
int check_flags(const char *flag);
int count_lines(const char *filename);
int count_words(const char *filename);
bool file_check(const char *file);
bool file_error(FILE *fp, int error_code);
void help_and_exit(void);

/* definitions */

inline int ccwc(const int argc, char **argv)
{
	if ( argc < min_args ) help_and_exit();

	const char *flag = argv[1];
	char *filename = argv[2];
	const int command = check_flags(flag);

	if (command == error || file_check(filename))
		return EXIT_FAILURE;

	const int count = counter(command, filename);
	printf("%2d: %s\n", count, filename);

	return EXIT_SUCCESS;
}

inline int counter(const int command, const char *file)
{
	switch (command)
	{
	case 0:		return count_bytes(file);
	case 1:		return count_lines(file);
	case 2:		return count_words(file);
	case 3:		return count_chars(file);
	default:	return error;
	};
}

inline int count_bytes(const char *filename)
{
	unsigned char c;
	int byte_count = 0;
	FILE *fb = fopen(filename, "rb");

	while (fread(&c, sizeof(char), 1, fb) > 0) ++byte_count;

	fclose(fb);

	return byte_count;
}

inline int count_chars(const char *filename)
{
	int char_count = 0;
	FILE *fr = fopen(filename, "r");

	while (fgetwc(fr) != WEOF) ++char_count;

	fclose(fr);

	return char_count;
}

inline int check_flags(const char *flag)
{
	if ( flag[0] != '-')			 return error;
	if ( strcmp( flag, "-c" ) == 0 ) return bytes;
	if ( strcmp( flag, "-h" ) == 0 ) help_and_exit();
	if ( strcmp( flag, "-l" ) == 0 ) return lines;
	if ( strcmp( flag, "-w" ) == 0 ) return words;
	if ( strcmp( flag, "-m" ) == 0 ) return chars;

	return error;
}

inline int count_lines(const char *filename)
{
	char c;
	int line_count = 0;
	FILE *fp = fopen(filename, "r");

	while ((c = fgetc(fp)) != EOF) if (c == '\n') ++line_count;

	fclose(fp);

	return line_count;
}

inline int count_words(const char *filename)
{
	char c;
	bool is_char = false;
	int word_count = 0;
	FILE *fp = fopen( filename , "r" );

	while ( ( c = fgetc( fp ) ) != EOF )
		if ( isspace( c ) ) is_char = false;
		else if ( !is_char ) is_char = true, ++word_count;

	fclose( fp );

	return word_count;
}

inline bool file_check(const char *filename)
{
	if (filename == NULL)	return file_error(NULL, EINVAL);

	FILE *fp = fopen(filename, "rb");

	if ( fp == NULL )	return file_error(fp, ENOENT);
	if ( ferror(fp) )	return file_error(fp, ( errno ? errno : EIO ));

	struct stat st;

	if ( fstat( fileno(fp), &st) != 0 )	return file_error(fp, errno);
	if ( S_ISDIR( st.st_mode ) )			return file_error(fp, EISDIR);

	if ( fseek( fp, 0, SEEK_END ) == 0 )	rewind(fp);
	else clearerr(fp);

	fclose(fp);

	return false;
}

inline bool file_error(FILE *fp, const int error_code)
{
	errno = error_code;
	printf("error! code: %d\n", error_code);

	if (fp != NULL)	fclose(fp);

	return true;
}

inline void help_and_exit(void)
{
	printf("%s\n", help_text);
	exit(EXIT_SUCCESS);
}

#endif // ccwc_h