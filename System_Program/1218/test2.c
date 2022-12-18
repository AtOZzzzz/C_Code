#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <stdlib.h>

void Fun();

int main()
{
    struct sigaction act;
    sigemptyset(&(act.sa_mask));
    sigaddset(&(act.sa_mask),SIGUSR1);
    // sigaddset(&(act.sa_mask),SIGCHLD);
    sigprocmask(SIG_BLOCK,&(act.sa_mask),NULL);
    act.sa_handler = Fun;
    act.sa_flags = 0;

    pid_t pid = fork();
    if(pid == -1)
    {
        perror("fork failed");
        exit(-1);
    }
    else if(pid > 0)
    {   
        printf("i am father, my pid is:%d\n",getpid());
        sleep(1);
        // sigaction(SIGUSR1,&act,NULL);
        sigaction(SIGUSR1,&act,NULL);
        sigprocmask(SIG_UNBLOCK,&(act.sa_mask),NULL);
        if(waitpid(-1,NULL,0) == -1)
        {
            perror("waitpid failed");
        }
        printf("waitpid 回来了\n");
        sleep(999);
    }
    else
    {
        printf("i am child,my pid is:%d\n",getpid());

        sleep(50);
    }



    return 0;
}


void Fun(int num)
{
    printf("Fun函数执行了\n");
}