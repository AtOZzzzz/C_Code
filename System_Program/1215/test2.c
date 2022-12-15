#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>
#include <stdlib.h>

#define PAGE_SIZE 4096


int main()
{
    int shm_id = shmget(0x77,PAGE_SIZE*9,IPC_CREAT | 0666);
    if(shm_id == -1)
    {
        perror("test2 shm_id failed");
        exit(-1);
    }
    printf("test2 shm_id is:%d\n",shm_id);

    char* shm_mapping = NULL;
    shm_mapping =  shmat(shm_id,NULL,0);
    if(shm_mapping == (void*)-1)
    {
        perror("test2 shm_mapping failed");
        exit(-1);
    }

    int copy_jpg_fd = open("./copy.jpg",O_RDWR | O_CREAT | O_TRUNC,0777);
    if(copy_jpg_fd == -1)
    {
        perror("test2 open failed");
        exit(-1);
    }

    if(ftruncate(copy_jpg_fd,46155) == -1)
    {
        perror("test2 ftruncate failed");
        exit(-1);
    }

    char* copy_mapping = mmap(NULL,46155,PROT_READ | PROT_WRITE ,MAP_SHARED,copy_jpg_fd,0);
    if(copy_mapping == MAP_FAILED)
    {
        perror("test2 mapping failed");
        exit(-1);
    }

    memcpy(copy_mapping,shm_mapping,36155);

    return 0;
}