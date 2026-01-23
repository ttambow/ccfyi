/*

	In this step your goal is to parse a valid simple JSON object, specifically: ‘{}’

	and an invalid JSON file and correctly report which is which.

	So you should build a very simple lexer and parser for this step.

	Your program should report to the standard output stream a suitable message

	and exit with the code 0 for valid and 1 for invalid.

	It is conventional for CLI tools to return 0

	for success and between 1 and 255 for an error

	and allows us to combined CLI tools to create more powerful programs.

	Check out write your own wc tool for more on combing simple cli tools.

	You can test your code against the files in the folder tests/step1.

	Consider automating the tests so you can run them repeatedly as you progress through the challenge.

*/

#include "cjparse.h"

#define help_text "no option(s) provided. usage: program_name [ -h ] <file>"

char *step_one_object = "{}";

int cjparse(void);
FILE *open_file(const char *filename);



bool cjparse(char **argv)
{
	const char *flag = argv[1];
	const char *filename = argv[2];
	const int command = check_flags(flag);

	if (command == error || file_check(filename))
		return EXIT_FAILURE;

	const int count = counter(command, filename);
	printf("%2d: %s\n", count, filename);

	return EXIT_SUCCESS;


	FILE *file = open_file(filename);
	while (fgetwc(file) != WEOF) {};
	printf( "we will compare:\n%s\nto:\n%s\n" , step_one_object, "todo" );
	fclose(file);
}
	return EXIT_SUCCESS;
}


FILE *open_file(const char *filename)
{
	return fopen(filename, "r");
}

int main( int argc , char **argv )
{
	// if ( argc < 3 ) help_and_exit( );
	cjparse();
}
