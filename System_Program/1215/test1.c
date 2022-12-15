#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>

#define PAGE_SIZE 4096


int main()
{
    int shm_id = shmget(0x77,PAGE_SIZE*9,IPC_CREAT | 0666);
    if(shm_id == -1)
    {
        perror("shmget failed");
        exit(-1);
    }
    printf("shm_id is:%d\n",shm_id);


    char* shm_mapping = NULL;
    shm_mapping = shmat(shm_id,NULL,0);
    if(shm_mapping == (void*)-1)
    {
        perror("shm_mapping failed");
        exit(-1);
    }

    int jpg_fd = open("./1.jpg",O_RDWR);
    if(jpg_fd == -1)
    {
        perror("open jpg failed");
        exit(-1);
    }

    int jpg_len = lseek(jpg_fd,0,SEEK_END);
    printf("jpg len is:%d\n",jpg_len);

    char* jpg_mapping = NULL;
    jpg_mapping = mmap(NULL,jpg_len,PROT_READ | PROT_WRITE,MAP_SHARED,jpg_fd,0);
    if(jpg_mapping == MAP_FAILED)
    {
        perror("jpg mapping failed");
        exit(-1);
    }
    //printf("%s\n",jpg_mapping);

    memcpy(shm_mapping,jpg_mapping,jpg_len);


    struct stat test;
    stat("./1.jpg",&test);
    printf("___%ld___",test.st_size);

    return 0;
}