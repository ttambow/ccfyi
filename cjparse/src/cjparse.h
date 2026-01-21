#ifndef cjparse_h
#define cjparse_h

#define _POSIX_C_SOURCE 200809L

#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <wchar.h>

#define help_text "no option(s) provided. usage: program_name [ -c | -l | -m | -w ] <file>"

enum { error = -1, bytes, lines, words, chars };

/* declarations */

bool check_file(const char *file, FILE *return_file);
int check_flags(char *flag);
int count_bytes(char *filename);
int count_chars(char *filename);
int count_lines(char *filename);
int count_words(char *filename);
int file_error(FILE *fp, int error_code);
void print_count(int count, char *filename);
void print_help(void);
int run_count(int command, char *file);

/* definitions */

int check_flags(char *flag)
{
	if ( flag[0] != '-')		 return error;
	if ( strcmp( flag, "-h" ) == 0 ) print_help();
	if ( strcmp( flag, "-c" ) == 0 ) return bytes;
	if ( strcmp( flag, "-l" ) == 0 ) return lines;
	if ( strcmp( flag, "-w" ) == 0 ) return words;
	if ( strcmp( flag, "-m" ) == 0 ) return chars;
	else				 return error;
}

bool check_file(const char *file, FILE *return_file)
{
	if (file == NULL || return_file == NULL) return file_error(NULL, EINVAL);
	
	FILE *fp = fopen(file, "rb");
	
	if ( fp == NULL ) return file_error(fp, ENOENT);
	if ( ferror(fp) ) return file_error(fp, ( errno ? errno : EIO ));
	
	struct stat st;
	
	if ( fstat( fileno(fp), &st) != 0 )	return file_error(fp, errno);
	if ( S_ISDIR( st.st_mode ) ) 		return file_error(fp, EISDIR);
	if ( fseek( fp, 0, SEEK_END ) == 0 ) 	rewind(fp);
	else clearerr(fp);

	*return_file = *fp;
	return false;
}

int count_bytes(char *filename)
{
	int bc = 0;
	unsigned char c;
	FILE *fb = fopen(filename, "rb");
    	while (fread(&c, sizeof(char), 1, fb) > 0) ++bc;
	fclose(fb);
	return bc;
}

int count_chars(char *filename)
{
	int cc = 0;
	FILE *fr = fopen(filename, "r");
	while (fgetwc(fr) != WEOF) ++cc;
	fclose(fr);
	return cc;
}

int count_lines(char *filename)
{
	char c;
	int lc = 0;
	FILE *fp = fopen(filename, "r");
	while ((c = fgetc(fp)) != EOF) if (c == '\n') ++lc;
	fclose(fp);
	return lc;
}

int count_words(char *filename)
{
	enum { space, character };
	int wc = 0;
	int n = 0;
	char ch;
	FILE *fp = fopen(filename, "r");
	while ((ch = fgetc(fp)) != EOF) {
		if (isspace(ch)) 
			n = space;
		else if (!n) {
			n = character;
			++wc;
		}
	}
	fclose(fp);
	return wc;
}

int file_error(FILE *fp, int error_code)
{
	errno = error_code;
	if (fp) fclose(fp);
	return true;
}

void print_count(int count, char *filename)
{
	printf("%d: %s\n", count, filename);
}

void print_help(void)
{
	printf("%s\n", help_text);
	exit(EXIT_SUCCESS);
}

int run_count(int command, char *file)
{
	switch (command) {
		case  0: return count_bytes(file);
		case  1: return count_lines(file);
		case  2: return count_words(file);
		case  3: return count_chars(file);
		default: return error;
	};
}

#endif // cjparse.h
