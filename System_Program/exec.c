#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
    pid_t process = fork();
    if(process == -1)
    {
        perror("fork failed");
        exit(-1);
    }
    else if(process > 0)  //father
    {
        int count = 5;
        while (count--)
        {
            printf("do something\n");
            sleep(1);
        } 
    }
    else//child
    {
        execl("./test","./test",NULL);              //exec函数族要执行.out文件
        perror("execl failed");
        exit(-1);
    }


    return 0;
}