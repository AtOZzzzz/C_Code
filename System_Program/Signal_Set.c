#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

// void Fun(int num);


// int main()
// {
//     if(signal(SIGUSR1,Fun) == SIG_ERR)
//     {
//         perror("signal failed");
//         exit(-1);
//     }

//     sigset_t set;
//     if( (sigemptyset(&set)) == -1)
//     {
//         perror("sigemptyset failed");
//         exit(-1);
//     }

//     if( (sigaddset(&set,SIGUSR1)) == -1)
//     {
//         perror("sigaddset failed");
//         exit(-1);
//     }

//     if( (sigismember(&set,SIGUSR1)) == 0)
//     {
//         perror("sigismember failed");
//         exit(-1);
//     }

//     if( (sigprocmask(SIG_BLOCK,&set,NULL)) == -1)
//     {
//         perror("sigprocmask failed");
//         exit(-1);
//     }

//     size_t i = 0;
//     printf("my process id is:%d\n",getpid());
//     for ( i = 0; i < 6; i++)
//     {
//         printf("hahhaha\n");
//         sleep(5);
//     }

//     if( (sigprocmask(SIG_UNBLOCK,&set,NULL)) == -1)
//     {
//         perror("sigprocmask unblock failed");
//         exit(-1);
//     }
    

//     return 0;
// }

// void Fun(int num)
// {
//     printf("do something\n");
// }


void Fun(int num);


int main()
{
    size_t i ;
    for ( i = 1; i <= SIGRTMAX; i++)
    {
        if( i == 32 || i == 33 || i == 9 || i == 19)
        {
            continue;
        }
        if (signal(i,Fun) == SIG_ERR)
        {
            perror("signal failed");
            exit(-1);
        }      
    }
    
    sigset_t set;

    if(sigemptyset(&set) == -1)
    {
        perror("sigemptyset failed");
        exit(-1);
    }

    if(sigfillset(&set) == -1)
    {
        perror("sigfillset failed");
        exit(-1);
    }

    if(sigprocmask(SIG_BLOCK,&set,NULL) == -1)
    {
        perror("sigprocmask failed");
        exit(-1);
    }

    for ( i = 1; i <= SIGRTMAX; i++)
    {
        if( i == 32 || i == 33 || i == 9 || i == 19)
        {
            continue;
        }
        if (kill(getpid(),i) == -1)
        {
            perror("kill(send signal) failed");
            exit(-1);
        }      
    }
    
    if(sigprocmask(SIG_UNBLOCK,&set,NULL) == -1)
    {
        perror("sigprocmask unblock failed");
        exit(-1);
    }

    
    return 0;
}


void Fun(int num)
{
    printf("%d信号执行了\n",num);
}