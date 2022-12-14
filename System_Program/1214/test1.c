#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

struct msg_buf
{
    long mtype;
    char mtext[20];
};

void Fun(int num);

int main()
{
    // key_t key = ftok("../",1);  //ftok到底是干嘛的？key有什么屌用？
    // if(key == -1)
    // {
    //     perror("ftok failed");
    //     exit(-1);
    // }
    // printf("key is:%d\n",key);
    // if(signal(SIGINT,Fun) == SIG_ERR)
    // {
    //     perror("set signal failed");
    //     exit(-1);
    // }

    int msg_que_id = msgget(0x99, IPC_CREAT | 0666); // 可行
    // int msg_que_id = msgget(key,IPC_CREAT | 0666);
    if (msg_que_id == -1)
    {
        perror("msgget failed");
        exit(-1);
    }
    printf("msg_que_id is:%d\n", msg_que_id);

    struct msg_buf buf1;

    pid_t pid = fork();
    if (pid == -1)
    {
        perror("fork failed");
        exit(-1);
    }
    else if (pid > 0)
    {
        // char tmp_arr[] = "A hello";
        // struct msg_buf buf1;
        memset(&buf1, 0, sizeof(buf1));
        buf1.mtype = 1;
        while (1)
        {
            printf("请输入发给test2的信息:");
            scanf("%s", buf1.mtext);

            // if(msgsnd(msg_que_id,&buf1,sizeof(buf1) - sizeof(long),0) == -1) //两种都行
            if (msgsnd(msg_que_id, &buf1, sizeof(buf1) - sizeof(long), 0) == -1)
            {
                perror("msgsnd failed");
                exit(-1);
            }
            // (*tmp_arr)++;
            // sleep(2);
        }
    }
    else
    {
        // struct msg_buf buf1;
        // sleep(10);
        memset(&buf1, 0, sizeof(buf1));
        int res = 0;
        // int i = 0;
        while (1)
        {
            res = msgrcv(msg_que_id, &buf1, (sizeof(buf1) - sizeof(long)), 2, 0);
            if (res == -1) // 没数据就自动堵塞。读一个删除一个
            {
                perror("msgrcv failed");
                exit(-1);
            }
            // printf("res is:%d, buf1.mtype is:%ld, buf1.mtext is:%s\n",res,buf1.mtype,buf1.mtext);
            printf("test2 speak:%s\n", buf1.mtext);
        }

        // if(msgctl(msg_que_id,IPC_RMID,NULL) == -1)
        // {
        //     perror("del msg failed");
        //     exit(-1);
        // }
    }

    return 0;
}

// void Fun(int num)
// {
//     printf("%d signal is coming\n",num);
//     execlp("killall","killall","-9","test",NULL);
//     perror("execlp failed");
//     exit(-1);
// }