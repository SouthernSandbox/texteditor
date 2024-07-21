/**
 * @file te_io.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-07-16
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "texteditor.h"
#include "stdarg.h"

char te_getchar(te_t te)
{
    return te->ops->getc();
}

/**
 * @brief 打印一个字符
 * @warning 该函数不会对偏移量进行递增
 * @param te 
 * @param c 
 * @return int 
 */
int te_putchar(te_t te, const char c)
{
    return te->ops->putc(c);
}

/**
 * @brief 打印一个字符
 * @warning 该函数会对偏移量进行递增
 * @param te 
 * @param c 
 * @return int 
 */
int te_ws_putchar(te_t te, const char c)
{
    te_ws_ofs(te)++;
    return te->ops->putc(c);
}

int te_puts(te_t te, const char *s)
{
    int i = 0;
    for(i = 0; s[i] != '\0'; i++)
        te_putchar(te, s[i]);
    return i;
}

int te_printf(te_t te, const char* fmt, ...)
{
    char buf[TE_TX_BUF_LEN] = {0};
    va_list arg;
    va_start(arg, fmt);
    vsnprintf(buf, TE_TX_BUF_LEN, fmt, arg);
    va_end(arg);
    return te_puts(te, buf);
}

int te_output(te_t te, const char* data, int len)
{
    for(int i = 0; i < len; i++)
        te_putchar(te, data[i]);
    return len;
}

int te_ws_output(te_t te, const char* data, int len)
{
    for(int i = 0; i < len; i++)
        te_ws_putchar(te, data[i]);
    return len;
}

int te_char_push(te_t te, te_line_buf_t lbuf, const char c)
{
    TE_UNUSED(te);
    if(lbuf->used < lbuf->len - 1)
    {
        lbuf->buf[lbuf->used++] = c;
        lbuf->buf[lbuf->used] = '\0';
    }
    return lbuf->used;
}

char te_char_pop(te_t te, te_line_buf_t lbuf)
{
    TE_UNUSED(te);
    if(lbuf->used > 0)
    {
        char c = lbuf->buf[--lbuf->used];
        lbuf->buf[lbuf->used] = '\0';
        return c;
    }
    return '\0';
}

int te_char_clear(te_t te, te_line_buf_t lbuf)
{
    TE_UNUSED(te);
    lbuf->buf[lbuf->used = 0] = '\0';
    return 0;
}
