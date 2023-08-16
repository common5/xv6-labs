#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

const int upp = 35;
int fst = 1;
int w, r;
int p[2];
int main(int argc, char **argv)
{
    if (fst)
    {
        fst = 0;
        pipe(p);
        w = p[1];
        r = p[0];
        for (int i = 2; i <= upp; i++)
        {
            write(w, (void *)&i, 4); // 把2到upp的所有整数都放到第一个进程里
        }
        close(w); // 不应该允许再次写入，因为管道的写端是开放给当前允许读的进程的前一个进程的
    }
    if (fork() == 0)
    {
        // 读取管道中第一个，它一定是质数
        int tmp, x;
        if (read(r, (void *)&tmp, 4))
        {
            printf("prime %d\n", tmp);
            pipe(p); // 创建新的管道
            w = p[1];
        }
        else
        {
            exit(0);
        }
        // printf("%d\n", tmp);
        while (read(r, (void *)&x, 4))
        {
            if (x % tmp != 0)
            {
                // printf("%d %d\n", x, tmp);
                write(w, (void *)&x, 4);
            }
        }
        r = p[0];
        close(w); // 不应该允许再次写入
        main(argc, argv);
    }
    else
    {
        int status;
        wait(&status);
        close(r);
    }
    exit(0);
}

// #include "kernel/types.h"
// #include "kernel/stat.h"
// #include "user/user.h"
// const int MAX_NUM = 35;
// int p1[2], fdr, fdw;
// long p, n;
// int is_first = 1;
// int main(int argc, char *argv[])
//{
// if (is_first == 1)
//{
// is_first = 0;
// pipe(p1);
// fdr = p1[0];
// fdw = p1[1];
// for (n = 2; n <= MAX_NUM; n++)
//{
// write(fdw, (void *)&n, 8);
// }
// close(fdw);
// }
// if (fork() == 0)
//{
// if (read(fdr, (void *)&p, 8))
//{
// printf("prime %d\n", p);
// pipe(p1);
// fdw = p1[1];
// }
// while (read(fdr, (void *)&n, 8))
//{
// if (n % p != 0)
// write(fdw, (void *)&n, 8);
// }
// fdr = p1[0];
// close(fdw);
// main(argc, argv);
// }
// else
//{
// wait((int *)0);
// close(fdr);
// }
// exit(0);
// }