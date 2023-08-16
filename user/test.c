#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

int main()
{
    int pid;

    pid = fork();
    int i = 1;
    if (pid == 0)
    {
        wait(0);
        i = 114;
        // 子进程执行的代码
        printf("%d这是子进程\n", pid);
        int x = fork();
        if (x == 0)
        {
            i--;
            printf("%d这是子进程\n的子进程\n", i);
        }
        // 子进程的其他操作
    }
    else
    {
        wait(0);

        i++;
        // 父进程执行的代码
        printf("%d这是父进程\n", i);
        // 父进程的其他操作
    }

    exit(0);
}
