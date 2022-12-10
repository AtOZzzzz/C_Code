/*
    短整型和长整形也有自己的输出格式  如：%hd %ho %hu %hx
                                      %ld %lo %lu %lx

    十六进制表示方式可以大写         如：-0XB = -0xb；

    2e3   为2*10的三次方

                                     
*/



#include<stdio.h>

int main()
{
    short a = 10;

    printf("%d ",a);
    printf("%hd ",a);
    printf("%o ",a);
    printf("%ho ",a);
    printf("%x ",a);
    printf("%hx ",a);

    printf("%hu ",a);

    int x = -0XB;
    printf("%d",x);


    return 0;
}