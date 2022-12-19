#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

void Routine1 (void * arg)
{
    printf("这里是取消处理函数1 ...\n");
}

void Routine2 (void * arg)
{
    printf("这里是取消处理函数2 ...\n");
}

void Routine3 (void * arg)
{
    printf("这里是取消处理函数3 ...\n");
}


void * Func (void * arg)
{
    // 设置的好当我们收到取消请求时的处理方法  （把出错处理函数入 【栈】 ）
    pthread_cleanup_push(Routine1 ,NULL );
    pthread_cleanup_push(Routine2 ,NULL );
    pthread_cleanup_push(Routine3 ,NULL );


    // while(1)
    // {
        sleep(1);
        printf("我是xxx线程, 是兄弟就来砍我....\n");
    // }

    // 有入栈就必须有出站函数， 而且他们必须在同一层的代码块中
    pthread_cleanup_pop(0);  // 参数为0的出栈并不执行 压栈的函数 
    pthread_cleanup_pop(1);  // 参数为非零 出栈并执行 压栈的函数 
    pthread_cleanup_pop(0);

    sleep(10);
    
    return NULL ;
}

int main(int argc, char const *argv[])
{
    
    pthread_t tid ;

    pthread_create(&tid , NULL , Func , NULL  );    

    sleep(5);

    // 发送取消请求
    pthread_cancel( tid );

    if(pthread_join( tid , NULL ))
    {
        printf("join error。。。。");
        return -1 ;
    }

    printf("子函数线程已经退出....\n");


    return 0;
}
