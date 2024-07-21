/**
 * @file te_file_ops.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-07-16
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "texteditor.h"

/**
 * @brief 打开指定路径的文件
 * @param te 
 * @param name 
 * @return int 
 */
te_fd_t te_file_open(te_t te, const char *path)
{
    te->file.fd = te->ops->open(path);
    return te->file.fd;
}

/**
 * @brief 读取指定长度的内容
 * @param te 
 * @param buf 
 * @param len 
 * @return int 
 */
int te_file_read(te_t te, te_fd_t fd, void* buf, int len)
{
    return te->ops->read(fd, buf, len);
}

/**
 * @brief 写入指定长度的内容
 * @param te 
 * @param buf 
 * @param len 
 * @return int 
 */
int te_file_write(te_t te, te_fd_t fd, void* buf, int len)
{
    return te->ops->write(fd, buf, len);
}

/**
 * @brief 设置文件读写指针
 * @param te 
 * @param ofs 
 * @return int 
 */
int te_file_lseek(te_t te, te_fd_t fd, int ofs)
{
    return te->ops->lseek(fd, ofs);
}

/**
 * @brief 关闭文件
 * @param te 
 * @return int 
 */
int te_file_close(te_t te, te_fd_t fd)
{
    te->ops->close(fd);
    te->file.fd = -1;
    return 0; 
}

/**
 * @brief 移除文件
 * @param te 
 * @param name 
 * @return int 
 */
int te_file_remove(te_t te, const char* name)
{
    te->ops->remove(name);
    return 0;
}

/**
 * @brief 文件重命名
 * @param te 
 * @param old_name 
 * @param new_name 
 * @return int 
 */
int te_file_rename(te_t te, const char* old_name, const char* new_name)
{
    te->ops->rename(old_name, new_name);
    return 0;
}

/**
 * @brief 检查目标文件是否存在
 * @param te
 * @param path
 * @return int
 */
int te_file_exist(te_t te, const char* path)
{
    return te->ops->exist(path);
}



/**
 * @brief 从目标文件的指定位置处加载一定长度的内容到工作区
 * @param te 
 * @param name 
 * @param pos 
 * @param len 
 * @return int 
 */
int te_file_load(te_t te, const char* path, int pos, int len)
{
    /** 打开文件，获取文件描述符 **/
    te_file_t file = &te->file;
    file->name = path;
    file->fd = te_file_open(te, path);
    if(file->fd < 0)
        return -1;      // 文件打开失败

    /** 设置读写位置 **/
    te_file_lseek(te, file->fd, pos);

    /** 从文件中读取数据到工作区 **/
    len = (len > te_ws_len(te)) ? te_ws_len(te) : len;
    file->len = te_file_read(te, file->fd, te_ws_buf(te), len);
    te_ws_used(te) += file->len;
    te_ws_lines(te) = te_get_char_count(te, 0, te_ws_used(te) - 1, '\n') + 1;
    te_ws_lines_max(te) = te_ws_lines(te);

    /** 关闭文件 **/
    te_file_close(te, file->fd);

    return 0;
}

/**
 * @brief 将工作区中的内容同步到文件中
 * @param te 
 * @return int 
 */
int te_file_sync(te_t te, const char* path)
{
    if(te_file_exist(te, path) == 0)    // 指定的文件存在
    {
        char temp_name[32] = {0};
        snprintf(temp_name, sizeof(temp_name), "temp_%s", path);
        te_fd_t old = te_file_open(te, path);
        if(old < 0)
            return -1;
        te_fd_t neo = te_file_open(te, temp_name);
        if(neo < 0)
            return -1;

        /** 先将文件前面部分移动到新文件中 **/
        for(int i = 0; i < te_file_pos(te); ++i)
        {
            char c;
            te_file_read(te, old, &c, 1);
            te_file_write(te, neo, &c, 1);
        }

        /** 将修改的内容写入新的文件中，而旧文件中的内容则会被忽略 **/
        int wr_len = te_ws_used(te) - 1;        // 写入时忽略 '\0'，因为 used 中包含了 '\0'
        te_file_write(te, neo, te_ws_buf(te), wr_len);                  // 向新文件写入工作区的内容
        te_file_lseek(te, old, te_file_pos(te) + te_file_len(te));      // 将旧文件的读写指针偏移，偏移到文件剩余未修改的部分

        /** 先将文件后面部分移动到新文件中 **/
        while(1)
        {
            char c;
            int ret = te_file_read(te, old, &c, 1);
            if(ret <= 0)
                break;
            te_file_write(te, neo, &c, 1);
        }

        te_file_close(te, old);
        te_file_close(te, neo);

        te_file_remove(te, path);
        te_file_rename(te, temp_name, path);

        te_file_fd(te) = neo;
    }
    else        // 指定的文件不存在
    {
        if(path == NULL)
            path = "./te_temp.txt";
        te_file_fd(te) = te_file_open(te, path);      // 创建一个新文件
        if(te_file_fd(te) < 0)
            return -1;
        te_file_write(te, te_file_fd(te), te_ws_buf(te), te_ws_used(te) - 1);   // 写入时忽略 '\0'，因为 used 中包含了 '\0'
        te_file_close(te, te_file_fd(te));
    }

    return 0;
}

























