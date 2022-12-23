#ifndef _THREAD_H_
#define _THREAD_H

#include "kernel_list.h"



#define TID_MAX 20
#define TASK_MAX 50


typedef struct thread_pool
{   
    bool shutdown;

    unsigned int tasks_num;

    unsigned int pthread_num;
    
    pthread_mutex_t mutex_var;  //互斥锁变量
   
    pthread_cond_t cond_var;    //
   
    host_node* task_list;

    pthread_t* tids;

}thread_pool;



bool Init_Pool(thread_pool* pool,unsigned int pthread_num);

void* Routine(void* arg);

void Handler(void* arg);

bool Add_Task(thread_pool *pool, void *(*task_fun)(void *), void *arg);

int Add_Thread(thread_pool* pool,unsigned int add_num);

int Remove_Thread(thread_pool *pool, unsigned int remove_num);

bool Destroy_Pool(thread_pool* pool);

void* Task_Fun(void* arg);
#endif