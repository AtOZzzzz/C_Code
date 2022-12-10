#include<stdio.h>
void a();
void b();
void c();

int main()
{
    // int int_data;
    // printf("请输入一个两位数");
    // scanf("%d",&int_data);


    // printf("这个数的十位是%d",int_data/10);

    // printf("这个数的个位是%d",int_data%10);

    c();

    return 0;
}

void c()
{
    long year;
    printf("请输入今年的年份");
    scanf("%ld",&year);

    if(year % 400 == 0 || (year % 4 == 0 && year % 100 != 0))
    {
        printf("该年为闰年");
    }
    else
    {
        printf("该年为平年");
    }
}














void b()
{
    int int_data1,int_data2;
    printf("请输入两个数");
    scanf("%d%d",&int_data1,&int_data2);
    if (int_data1 > int_data2)
    {
        printf("%d大于int_data2%d",int_data1,int_data2);
    }
    else
    {
        printf("%d小于int_data2%d",int_data1,int_data2);
    }
    
}

void a()
{
    int y,x = 10 ;

    y = ++x;
    printf("现在x是%d,y是%d",x,y);
    // y = x++;
    // printf("现在x是%d,y是%d",x,y);
}