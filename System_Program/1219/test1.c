#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>

void *Fun(void *);

struct test
{
    int data;
    char arr[20];
};

int flag = 0;

int main()
{
    struct test t1 = {66, "hello world"};

    pthread_t pthread;
    int errernum = pthread_create(&pthread, NULL, Fun, &t1);
    if (errernum != 0)
    {
        printf("pthread creat failed:%s\n", strerror(errernum));
    }

    // while (1)
    // {
    //     if (flag == 1)
    //     {
    //         sleep(1);
    //         printf("我是main线程\n");
    //         flag = 0;
    //     }
    // }
    time_t start , end;

    int *p = NULL;
    struct timespec timess;
   
    // int pthread_timedjoin_np(pthread_t thread, void **retval, // 在一定时间阻塞等待
    //                          const struct timespec *abstime); // 需要设置的是绝对时间
    // pthread_join(pthread,(void**)(&p));
    // errernum = pthread_tryjoin_np(pthread,(void**)(&p));
    // int sec = clock_gettime(CLOCK_REALTIME,&timess);
    // if(sec == -1)
    // {
    //     perror("gettime\n");
    // }
    time(&start);
    printf("%ld\n",start);
    timess.tv_sec = start + 5;
    printf("%ld\n",timess.tv_sec);
    //timess.tv_sec +=10;
    //printf("%ld\n",timess.tv_sec);
    timess.tv_nsec = 0;
    // timess.tv_sec += 5;
    errernum = pthread_timedjoin_np(pthread, (void **)(&p),&timess);
    if (errernum)
    {
        printf("tryjoin failed:%s\n", strerror(errernum));
    }
    time(&end);
    printf("时间差为%lf\n",difftime(end,start));


    printf("%d\n",*p);

    return 0;
}

void *Fun(void *p)
{
    // while (1)
    // {
    //     if (flag == 0)
    //     {
    //         sleep(1);
    //         printf("我是Fun线程\n");
    //         flag = 1;
    //     }
    // }
    printf("__%d__,__%s__\n", ((struct test *)p)->data, ((struct test *)p)->arr);

    printf("hah\n");

    static int num = 77;

    sleep(6);
    return (void *)&num;
}