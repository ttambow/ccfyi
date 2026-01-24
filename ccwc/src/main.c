#include "ccwc.h"

int main(const int argc, char **argv)
{
    if ( argc < min_args ) print_help_and_exit ();
    return ccwc(argv);
}