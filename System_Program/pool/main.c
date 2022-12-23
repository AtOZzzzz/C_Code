#include "thread.h"

int main()
{
    thread_pool* pool =(thread_pool*)calloc(1,sizeof(thread_pool));
    if(pool == NULL)
    {
        perror("pool malloc failed");
        exit(-1);
    }

    bool ret = false;
    ret = Init_Pool(pool,1);
    if(ret == false)
    {
        perror("init pool failed");
        exit(-1);
    }

    Add_Task(pool,Task_Fun,NULL);
    Add_Task(pool,Task_Fun,NULL);
    Add_Task(pool,Task_Fun,NULL);
    Add_Task(pool,Task_Fun,NULL);               
    Add_Thread(pool,2);
    printf("现在的线程数是：%d\n",Remove_Thread(pool,0));           //3
    sleep(20);

    Add_Thread(pool,2);
    Add_Task(pool,Task_Fun,NULL);               
    Add_Task(pool,Task_Fun,NULL);
    printf("现在的线程数是：%d\n",Remove_Thread(pool,0));       //5
    sleep(5);

    Remove_Thread(pool,3);
    printf("现在的线程数是：%d\n",Remove_Thread(pool,0));       //2
    Add_Task(pool,Task_Fun,NULL);
    Add_Task(pool,Task_Fun,NULL); 
    sleep(5);

    printf("准备销毁\n");
    Destroy_Pool(pool);
    // Add_Task(pool,Task_Fun,NULL);


    sleep(500);





    return 0;
}