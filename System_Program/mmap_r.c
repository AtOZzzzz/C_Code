#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <unistd.h>     //ftruncate
#include <string.h>


int main()
{
    umask(0000);
    int fd = open("./aa.txt",O_RDWR | O_CREAT ,0777);
    if(fd == -1)
    {
        perror("open failed");
        exit(-1);
    }

    if(ftruncate(fd,30) == -1)
    {
        perror("fruncate failed");
        exit(-1);
    }

    int len = lseek(fd,0,SEEK_END);
    printf("file len is:%d\n",len);

    char* p = NULL;
    p = (char*)mmap(NULL,len,PROT_READ | PROT_WRITE ,MAP_SHARED,fd,0);
    if(p == MAP_FAILED)
    {
        perror("mmap failed");
        exit(-1);
    }

    close(fd);

    

    while(1)
    {
        printf("%s\n",p);
        sleep(2);
    }
    munmap(p,len);

    return 0;
}