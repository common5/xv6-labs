#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

const char *lastname(char *path)
{
    char *p = path;
    while (*p)
        p++;
    while (*p != '/' && p != path)
        p--;
    return p == path ? p : ++p;
}
void find(char *path, char *name)
{
    char buf[512];
    int fd;
    struct dirent de;
    struct stat st;
    char *p;

    if ((fd = open(path, 0)) < 0)
    {
        fprintf(2, "无法打开 %s\n", path);
        return;
    }

    if (fstat(fd, &st) < 0)
    {
        fprintf(2, "无法打开 %s\n", path);
        close(fd);
        return;
    }

    if (st.type == T_FILE) // 文件则判断是否是所找文件
    {
        if (strcmp(lastname(path), name) == 0)
        {
            printf("%s\n", path);
            // printf("%s\n", path);
        }
    }
    else if (st.type == T_DIR) // 文件夹则查询所有的子项
    {
        if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf)
        {
            printf("find: 路径过长\n");
        }
        strcpy(buf, path);
        p = buf + strlen(buf);
        *p++ = '/';
        while (read(fd, &de, sizeof(de)) == sizeof(de))
        {
            if (de.inum == 0 || strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
            {
                continue;
            }
            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = 0; // 尾0
            find(buf, name);
        }
    }
    close(fd);
}
int main(int argc, char **argv)
{
    if (argc != 3)
    {
        printf("参数错误!\n");
    }
    else
    {
        find(argv[1], argv[2]);
    }
    exit(0);
}
