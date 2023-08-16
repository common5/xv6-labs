#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"
int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("参数错误！");
        exit(0);
    }

    // 复制一下参数
    char *param[MAXARG];
    for (int i = 1; i < argc; i++)
    {
        param[i - 1] = argv[i];
    }

    char buf[64];
    int x = 1;
    // 按行读取参数
    while (1)
    {
        x++;
        int loc = argc - 1;

        // 循环读取参数，并将参数添加在param的后方
        gets(buf, sizeof(buf));
        if (buf[0] == 0)
        {
            break;
        }
        param[loc] = buf;

        //  将buf中的参数根据空格分割填入param中
        for (char *p = buf; *p != 0; p++)
        {
            if (*p == ' ')
            {
                *p = 0;
                param[++loc] = p + 1;
            }
            else if (*p == '\n')
            {
                *p = 0;
            }
        }
        //  调用子程序执行命令
        if (fork() == 0)
        {
            exec(argv[1], param);
        }
    }
    wait(0);
    exit(0);
}
// echo h1 h2 | xargs echo b1 b2