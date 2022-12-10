#include<stdio.h>

int main()
{
    char str[ ]= "Hello";
    char *p  = str;
    int n=10;
      printf("%d ",sizeof(str))  ;
      printf("%d ",sizeof(p))  ;
      printf("%d\n",sizeof(n))  ;
    printf("==========================\n");
    int a = 10;
    char c = 65;
      
    int* pa = &a;
    char* pc = &c;

    printf("%p ",pa);
    printf("%p ",pc);
    printf("\n%d",sizeof(pa));
    printf("\n%d",sizeof(pa));

    short x[] = {0,5,6,7,9,21,9};
    char y[]  = {"jdshfushfh"};

    short* px = x;
    char*  py = y;
    printf("\n%d",sizeof(px));
    printf("\n%d",sizeof(py));
    printf("\n%d",sizeof(*px));
    printf("\n%d",sizeof(*py));
}