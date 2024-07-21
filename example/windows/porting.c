#include "stdio.h"
#include "conio.h"
#include "texteditor.h"


static char _getc(void)
{
    return getch();
}

static int _putc(char c)
{
    return putchar(c);
}            

static te_fd_t _open(const char* name)
{
    if(!name)
        return -1;

    FILE* file = fopen(name, "rb+");    // 以 bin 方式打开文件
    if(file == NULL)
    {
        FILE* file = fopen(name, "w");  // 若文件不存在则创建文件
        if(file == NULL)
            return -1;
        else
        {
            fclose(file);
            return _open(name);
        }
    }
    else
        return (te_fd_t) file;
}

static int _read(te_fd_t fd, char* buf, int len)
{
    FILE* file = (FILE*) fd;
    int size = fread(buf, 1, len, file);
    return size;
}

static int _write(te_fd_t fd, const char* buf, int len)
{
    FILE* file = (FILE*) fd;
    int size = fwrite(buf, 1, len, file);
    return size;
}

static int _lseek(te_fd_t fd, int offset)
{
    FILE* file = (FILE*) fd;
    return fseek(file, offset, SEEK_SET);
}

static int _close(te_fd_t fd)
{
    FILE* file = (FILE*) fd;
    return fclose(file);
}

static int _remove(const char* name)
{
    remove(name);
    return 0;
}

static int _rename(const char* old_name, const char* new_name)
{
    rename(old_name, new_name);
    return 0;
}

static int _exist(const char* path)
{
    if(path == NULL)
        return -1;

    FILE* file = fopen(path, "r");
    if(file == NULL)
        return -1;
    else
    {
        fclose(file);
        return 0;
    }
}

static int _quit(te_t te)
{
    TE_UNUSED(te);
    exit(0);
    return 0;
}


static struct te_ops ops = 
{
    .getc = _getc, 
    .putc = _putc, 
    .open = _open,
    .read = _read,
    .write = _write,
    .lseek = _lseek,
    .close = _close,
    .remove = _remove,
    .rename = _rename,
    .exist = _exist,
    .quit = _quit,
};


te_ops_t get_te_ops(void)
{
    return &ops;
}
