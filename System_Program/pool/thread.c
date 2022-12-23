#include "thread.h"

bool Init_Pool(thread_pool *pool, unsigned int pthread_num)
{
    assert(pool != NULL);

    if (pthread_num < 1 || pthread_num > TID_MAX)
    {
        printf("初始化线程数输入错误\n");
        return false;
    }

    bool shutdown = false;

    pthread_mutex_init(&pool->mutex_var, NULL);
    pthread_cond_init(&pool->cond_var, NULL);

    pool->task_list = Creat_Host();
    if (pool->task_list == (host_node *)-1)
    {
        perror("task_list head creat failed");
        return false;
    }

    pool->tids = (pthread_t *)calloc(1, TID_MAX * sizeof(pthread_t));
    if (pool->tids == NULL)
    {
        perror("tids calloc failed");
        return false;
    }

    int ret = 0;
    int i;
    for (i = 0; i < pthread_num; i++)
    {
        ret = pthread_create((pool->tids) + i, NULL, Routine, (void *)pool);
        if (ret != 0)
        {
            perror("pthread creat failed");
            return false;
        }
    }
    pool->pthread_num = pthread_num;

    return true;
}

void *Routine(void *arg)
{
    assert(arg != NULL);

    thread_pool *pool = (thread_pool *)arg;
    host_node *temp = NULL;

    while (1)
    {
        pthread_cleanup_push(Handler, (void *)(&(pool->mutex_var)));
        pthread_mutex_lock(&(pool->mutex_var));

        // 这里不能用if
        while (pool->tasks_num == 0 && pool->shutdown == false)
        {
            // 进入条件变量等待队列，pthread_cond_wait自己会有一次解锁动作，出去自己也会上锁。
            pthread_cond_wait(&(pool->cond_var), &(pool->mutex_var));
        }

        if (pool->tasks_num == 0 && pool->shutdown == true)
        {
            pthread_mutex_unlock(&(pool->mutex_var));
            pthread_exit(NULL);
        }

        // 有任务的情况且没关闭
        temp = list_entry(pool->task_list->para.next, host_node, para);
        list_del(&(temp->para));
        pool->tasks_num--;

        pthread_mutex_unlock(&(pool->mutex_var));
        pthread_cleanup_pop(0);

        pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
        (temp->task)(temp->arg);
        pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);

        free(temp);
    }

    pthread_exit(NULL);
}

void Handler(void *arg)
{
    pthread_mutex_unlock((pthread_mutex_t *)arg);

    // pthread_exit(NULL);
}

bool Add_Task(thread_pool *pool, void *(*task_fun)(void *), void *arg)
{
    assert(pool != NULL);

    pthread_mutex_lock(&(pool->mutex_var));

    if (pool->tasks_num < TASK_MAX)
    {
        Tail_Inser(pool->task_list, task_fun, arg);
        pool->tasks_num++;
    }
    pthread_mutex_unlock(&(pool->mutex_var));

    pthread_cond_broadcast(&(pool->cond_var));

    return true;
}

int Add_Thread(thread_pool *pool, unsigned int add_num)
{
    assert(pool != NULL);

    int ret = 0;
    int i;
    for (i = 0; i < add_num && pool->pthread_num < TID_MAX; i++)
    {
        ret = pthread_create(pool->tids + pool->pthread_num, NULL, Routine, (void *)pool);
        if (ret != 0)
        {
            perror("pthread creat failed");
            return i;
        }
        pool->pthread_num++;
    }

    return add_num;
}

int Remove_Thread(thread_pool *pool, unsigned int remove_num)
{
    assert(pool != NULL);
    if (remove_num >= pool->pthread_num)
    {
        printf("要移除的线程数量大于等于实际数量，留一条线程，其余清空\n");
        remove_num = pool->pthread_num - 1;
    }

    int ret = 0;
    int i;
    for (i = 0; i < remove_num; i++)
    {
        ret = pthread_cancel(*(pool->tids + pool->pthread_num - 1 - i));
        if (ret != 0)
        {
            printf("cancel failed:%s\n", strerror(errno));
            return pool->pthread_num;
        }
        pool->pthread_num--;
    }

    return pool->pthread_num;
}

bool Destroy_Pool(thread_pool *pool)
{
    pool->shutdown = true;
    
    /*线程全都在条件变量等待队列睡觉，线程无法结束，导致接合一直堵塞，无法销毁，唤醒他们*/
    pthread_cond_broadcast(&(pool->cond_var));

    pthread_cond_destroy(&(pool->cond_var));

    int ret = 0;
    int i;
    for (i = 0; i < pool->pthread_num; i++)
    {
        ret = pthread_join(*(pool->tids + i),NULL);
        if (ret != 0)
        {
            printf("destory pool failed:%s,%s\n", strerror(errno), strerror(ret));
            return false;
        }
    }
    // while (1)
    // {
    //     if (pool->tasks_num == 0)
    //     {
    //         for (i = 0; i < pool->pthread_num; i++)
    //         {
    //             ret = pthread_cancel(*(pool->tids + i));
    //             printf("___%d___\n", ret);
    //             if (ret != 0)
    //             {
    //                 printf("destory pool failed:%s,%s\n",strerror(errno),strerror(ret));
    //                 return false;
    //             }
    //             printf("取消线程成功\n");
    //         }
    //         break;
    //     }
    // }
    Ruin_List(pool->task_list);
    free(pool->tids);
    free(pool);
    printf("___%d___\n", __LINE__);

    // printf("___%d___\n", __LINE__);
    // printf("___%d___\n", __LINE__);
    return true;
}

void *Task_Fun(void *arg)
{
    // printf("______%d_____\n",*((int*)arg));
    printf("_____runing______\n");
}