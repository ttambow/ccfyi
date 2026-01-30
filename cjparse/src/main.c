

#include "cjparse.h"

#define min_args 2

struct file_data_t {
	FILE *file;
	int size;
};

char *step_one_object = "{}";

struct file_data_t open_file(const char *file_name);

int cjparse(char **argv);
void handle_error(int error_code);

int cjparse(char **argv)
{
	const char *file_name = argv[1];
	printf("filename is: %s\n", file_name);
	const struct file_data_t file_data = open_file( file_name );
	char buffer[file_data.size];
	fgets(buffer, (int) sizeof(buffer), file_data.file);
	printf("buffer is: %s\n", buffer);

	return fclose(file_data.file);
}

void handle_error(const int error_code)
{
	errno = error_code;
	printf("error! code: %d\n", error_code);

	// todo: make file global
	// if (file != NULL)
	// 	close(file);

	exit(error_code);
}

struct file_data_t open_file(const char *file_name)
{
	if (file_name == NULL) handle_error(EINVAL);

	FILE *file = fopen(file_name, "r");

	if ( file == NULL )	handle_error(ENOENT);
	if ( ferror(file) )	handle_error(errno);

	struct stat st;

	if ( fstat( fileno(file), &st) != 0 ) handle_error(errno);
	if ( S_ISDIR( st.st_mode ) ) handle_error(EISDIR);

	if ( fseek(file, 0, SEEK_END) == 0)
		rewind(file);
	else
	{
		clearerr(file);
		fclose(file);
		exit(EXIT_FAILURE);
	}

	fseek(file, 0, SEEK_END);

	const int file_size = (int) ftell(file) + 1;

	fseek(file, 0, SEEK_SET);

	const struct file_data_t file_data = {
		.file = file,
		.size = file_size
	};

	return file_data;
}

/*
	./cjparse valid.json
	"valid.json" is valid json
	./cjparse invalid.json
	"invalid.json" is invalid json
*/
int main(const int argc , char **argv)
{
	// for (int i = 0; i < argc; i++)
	// 	printf("arg[%d] is: %s\n", i, argv[i]);
	if ( argc < min_args ) print_help();
	return cjparse(argv);
}

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