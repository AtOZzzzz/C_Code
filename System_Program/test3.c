#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>
#include <stdlib.h>


int main()
{
    int fd = 0;
    umask(0000);
    fd = open("./aa.txt",O_RDWR | O_CREAT | O_TRUNC,0777);
    if( fd == -1)
    {
        perror("open failed");
        exit(-1);
    }

    //创建空洞文件（要不然后面映射的时候，文件大小为0，会报bus error）
    if(ftruncate(fd,30) == -1)
    {
        perror("ftruncate failed");
        exit(-1);
    }

    int len = lseek(fd,0,SEEK_END);
    printf("fd len is:%d\n",len);

    char* p = NULL;
    p = (char*)mmap(NULL,30,O_RDWR,MAP_SHARED,fd,0);
    if(p == MAP_FAILED)
    {
        perror("mapping failed");
        exit(-1);
    }

    strcpy(p,"hello world");

    //似乎MMU帮你映射的时候直接映射了4个页（1个页4KB）的大小；
    //strcpy(p + 4096*4 - strlen("hello world"),"hello world");

    printf("%s\n",p);

    return 0;
}