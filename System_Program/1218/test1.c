#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>

void Fun(int num);

int main()
{
    pid_t pid = 0;

    int i = 0;
    for (i = 0; i < 10; i++)
    {
        if ((pid = fork()) == -1)
        {
            perror("fork failed");
            exit(-1);
        }
        else if (pid == 0)
        {
            break;
        }
    }

    if (i == 10)
    {
        printf("i am father,my pid is:%d,i am doing something\n", getpid());

        struct sigaction act;
        act.sa_handler = Fun;
        // sigset_t mask;
        sigemptyset(&(act.sa_mask));
        // act.sa_mask = mask;
        act.sa_flags = SA_NODEFER;
        sigaction(SIGINT, &act, NULL);

        while (1)
        {
            ;
        }
        // sleep(2);
    }
    else
    {
        printf("i am child,my pid is:%d\n", getpid());
    }

    return 0;
}

void Fun(int num)
{
    printf("Fun函数执行\n");
    sleep(5);
    printf("睡完了\n");

    // pid_t wpid = 0;

    // wpid = waitpid(-1,NULL,0);
    // if(wpid == -1)
    // {
    //     perror("wait failed");
    //     exit(-1);
    // }
    // printf("_____________catch siganl:%d,waitpid is:%d\n", num, wpid);
    // while ((wpid = waitpid(-1, NULL, 0)) != -1)
    // {
    //     printf("_____________catch siganl:%d,waitpid is:%d\n", num, wpid);
    // }
}