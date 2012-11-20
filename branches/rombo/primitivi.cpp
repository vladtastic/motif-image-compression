#include <stdio.h>

int main (void)
{
    printf ("unsigned char          %d\n", (int) sizeof (unsigned char));
    printf ("short int     %d\n", (int) sizeof (short int));
    printf ("int           %d\n", (int) sizeof (int));
    printf ("long int      %d\n", (int) sizeof (long int));
    printf ("long long int %d\n", (int) sizeof (long long int));
    printf ("float         %d\n", (int) sizeof (float));
    printf ("double        %d\n", (int) sizeof (double));
    printf ("long double   %d\n", (int) sizeof (long double));
    return 0;
}