#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdarg.h>
#include <unistd.h>
#include <errno.h>

int NX,NY;
void print_and_exit(char *, ...);

int main( int argc, char **argv )
{

   switch (argc){
     case 3:
       sscanf(argv[1],"%d",&NX);
       sscanf(argv[2],"%d",&NY);
       break;
     default:
       print_and_exit("Usage: %s NX NY\n",argv[0]);
   }

   int a,b,c;
    a=(NX-1)*(NY/2)+(NX-2)*((NY-1)/2);
    b=(NX-2)*(NY/2)+(NX-1)*((NY-1)/2);
    c=(NX-1)*(NY-2);

    printf("Type I dihedral = %d\n",a);
    printf("Type II dihedral = %d\n",b);
    printf("Type III dihedral = %d\n",c);
    printf("Total dihedral = %d\n",a+b+c);

    return 0;
}


void print_and_exit(char *format, ...)
{
    va_list list;
    va_start(list,format);
    vfprintf(stderr,format,list);
    va_end(list);
    exit(1);
}
