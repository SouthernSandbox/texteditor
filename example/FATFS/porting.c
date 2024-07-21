#include "stdio.h"
#include "stdlib.h"
#include "texteditor.h"
#include "ff.h"
#include "uart.h"

#define DISK_ID  "0:"

static char _getc(void)
{
    char c = 0;
    while(!uart_read(&c, 1));
    return c;
}

static int _putc(char c)
{
    return uart_write(&c, 1);
}            

static te_fd_t _open(const char* name)
{
    if(!name)
        return -1;

    char path[32] = {0};
    sprintf(path, DISK_ID "%s", name);
    FRESULT res;

    FIL* file = malloc(sizeof(FIL));
    if((res = f_open(file, path, FA_OPEN_ALWAYS | FA_READ | FA_WRITE)) == FR_OK)
        return (te_fd_t) file;
    else
    {
        free(file);
        return -1;
    }
}

static int _read(te_fd_t fd, char* buf, int len)
{
    UINT read_count;
    f_read((FIL*) fd, buf, len, &read_count);
    return read_count;
}

static int _write(te_fd_t fd, const char* buf, int len)
{
    UINT write_count;
    FIL* file = (FIL*) fd;
    f_write(file, buf, len, &write_count);
    return write_count;
}

static int _lseek(te_fd_t fd, int offset)
{
    FIL* file = (FIL*) fd;
    f_lseek(file, offset);
    return 0;
}

static int _close(te_fd_t fd)
{
    FIL* file = (FIL*) fd;
    f_close(file);
    free(file);
    return 0;
}

static int _remove(const char* name)
{
    char path[32] = {0};
    sprintf(path, DISK_ID "%s", name);
    f_remove(path);
    return 0;
}

static int _rename(const char* old_name, const char* new_name)
{
    char* old_path = malloc(64);
    sprintf(old_path, DISK_ID "%s", old_name);
    char* new_path = malloc(64);
    sprintf(new_path, DISK_ID "%s", new_name);
    f_rename(old_path, new_path);
    free(old_path);
    free(new_path);
    return 0;
}

static int _exist(const char* path)
{
    if(path == NULL)
        return -1;

    FRESULT res;

    FIL* file = malloc(sizeof(FIL));
    if((res = f_open(file, path, FA_OPEN_ALWAYS) == FR_OK))
    {
        f_close(file);
        free(file);
        return 0;
    }
    else
    {
        free(file);
        return -1;
    }
}

static int _quit(te_t te)
{
    TE_UNUSED(te);
    extern bool is_te_stop;
    is_te_stop = true;
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
