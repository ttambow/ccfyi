#include "ccwc.h"

int main(int argc, char **argv)
{
	if ( argc < 3 ) print_help();
	FILE *fp = NULL;
	char *flag = argv[1];
	char *filename = argv[2];
	int count = 0;
	int command = check_flags(flag);
	if ( command != error && check_file(filename, fp) ) {
		count = run_count(command, filename);
		print_count(count, filename);
		return EXIT_SUCCESS;
	}
	else return EXIT_FAILURE;
}
