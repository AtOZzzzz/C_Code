#include <stdio.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

struct msg_buf
{
    long mtype;
    char mtext[20];
};

int main()
{
    struct msg_buf buf;

    int msg_que_id = msgget(0x99, IPC_CREAT | 0666);
    if (msg_que_id == -1)
    {
        perror(" test2 msgget failed");
        exit(-1);
    }
    printf("msg_que_id is:%d\n", msg_que_id);

    pid_t pid = fork();
    if (pid == -1)
    {
        perror("test2 fork failed");
        exit(-1);
    }
    else if (pid > 0)
    {
        memset(&buf, 0, sizeof(buf));
        buf.mtype = 2;

        while (1)
        {
            printf("请输入发送给test1的信息:");
            scanf("%s", buf.mtext);

            if (msgsnd(msg_que_id, &buf, sizeof(buf) - sizeof(long), 0) == -1)
            {
                perror("test2 msgsnd failed");
                exit(-1);
            }
        }
    }
    else
    {
        memset(&buf, 0, sizeof(buf));

        while (1)
        {
            if (msgrcv(msg_que_id, &buf, sizeof(buf) - sizeof(long), 1, 0) == -1)
            {
                perror("test2 msgrcv failed");
                exit(-1);
            }

            // if (!(strcmp(buf.mtext, "quit")))
            // {
            //     if (msgctl(msg_que_id, IPC_RMID, NULL) == -1)
            //     {
            //         perror("del msg failed");
            //         exit(-1);
            //     }
            //     exit(0);
            // }
            printf("test1 speak:%s\n", buf.mtext);
        }
    }

    return 0;
}