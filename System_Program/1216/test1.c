#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

void Fun1(int num);

// int main()
// {
//     sigset_t set,old;

//     sigemptyset(&set);

//     sigaddset(&set,SIGINT);

//     sigismember(&set,SIGINT);

//     sigprocmask(SIG_BLOCK,&set,&old);
//     sigprocmask(SIG_UNBLOCK,&set,&old);

//     printf("%x\n",old);
// }

int main()
{
    // struct sigaction
    // {
    //     void (*sa_handler)(int);                     //注册函数指针
    //     void (*sa_sigaction)(int, siginfo_t *, void *);    //带数据的注册函数指针
    //     sigset_t sa_mask;                       //信号集
    //     int sa_flags;        //如果设置了SA_SIGINFO标志位，
                                //则会使用sa_sigaction处理函数，否则使用sa_handler处理函数
    //     void (*sa_restorer)(void);           //没用
    // };
    printf("pid is:%d\n",getpid());

    struct sigaction act;
    // act.sa_handler = Fun1;
    act.sa_handler = SIG_IGN;

    sigemptyset(&act.sa_mask);
    sigaddset(&act.sa_mask,SIGUSR1);

    act.sa_flags = 0;

    if(sigaction(SIGINT,&act,NULL) == -1)
    {
        perror("sigaction failed");
        exit(-1);
    }


    while(1)
    {
        ;
    }
    // int sigaction(int signum, const struct sigaction *act,struct sigaction *oldact);

    return 0;
}


void Fun1(int num)
{
    printf("第一个sleep执行\n");
    sleep(5);
    printf("Fun1 执行,%d signal catch\n",num);
    sleep(2);
}

void Fun2(int num)
{
    printf("Fun2 执行,%d signal catch\n",num);
}