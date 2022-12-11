#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
    int pipe_fd[2] = {0};
    int ret = 77;
    ret = pipe(pipe_fd);
    if (ret == -1)
    {
        perror("pipe failed");
        exit(-1);
    }

    pid_t process = fork();
    if (process == -1)
    {
        perror("fork failed");
        exit(-1);
    }
    else if (process > 0) // father
    {
         
        printf("%d-----%d\n",pipe_fd[0],pipe_fd[1]);

        close(pipe_fd[0]);
        write(pipe_fd[1], "hello world", sizeof("hello world"));
    }
    else // child
    {
        char arr[50] = "\0";
        close(4);
        read(3, arr, sizeof("hello world"));
        printf("%s\n", arr);
    }

    return 0;
}