#include <stdio.h> 
#include <stdlib.h> 

int main(int argc, char * argv[]) {
    int a, conv;

    conv=scanf("%*1[m]%n", &a);
    printf ("conv = %i a= %i\n", conv, a);

    return EXIT_SUCCESS;
} 
