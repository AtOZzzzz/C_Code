#include<stdio.h>
void b();
void c();
void d();

int main()
{
    
    d();

}


void d()
{
    int n = 0 ;
    while(n++<=2) printf("%d",n);
    // printf("%d",n);
    


}

























void c()
{
    int a =100;
    printf("%p\n",&a);
    // for (; a >= 0; a--)
    // {
    //     if(!(a % 2))
    //     printf("%d\n",a);
    //     //printf("%p\n",&a);
    // }


    while (!(a % 2))
    {
        printf("%d\n",a);
        a--;
    }
    
    
}




















void b()
{
    for(int a = 0; a < 3 ; a++)
    {
        for (int i = 0; i < 4; i++)
        {
        printf("*");
        }
        
        printf("\n");

    }
}


















void a()
{
    int a = 10; 
    int *p = &a;
    printf("%p\n",&a);

    if(a == 10)
    {
        int a = 50;

        printf("%d\n",*p);

        printf("%p\n",&a);

    }
}