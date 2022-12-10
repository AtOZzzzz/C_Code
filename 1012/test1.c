#include <stdio.h>
void test();


int main()
{
    test();
}


void test()
{
    int  a , b , x ;
    a = 67;
    b = 70;
    printf("请输入1个字符\n");
    scanf("%d",&x);

    if (x >= 30  && x <= 100)
    {
        putchar(a);
        // printf("%d",a);
    }

    else
    {
       putchar(b);
       //printf("%d",b);
    }
    




}