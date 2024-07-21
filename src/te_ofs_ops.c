/**
 * @file te_ofs_ops.c
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
 * @brief 检查偏移量是否有效，有效范围取决工作区的字节使用量 used.
 * @note 有效范围定义在 ofs∈[0, used]. 例如：
 *          ofs:   01234 5
 *          str:   hello[\0]
 *       used = 6, 而 ofs 最多只可操作到 ws[used - 1] 处，即 [\0] 的位置。
 * @param te
 * @param ofs
 * @return
 */
bool te_is_ofs_valid(te_t te, te_ofs_t ofs)
{
    return (ofs >= 0 && ofs < te_ws_used(te));
}

/**
 * @brief 检查 ofs 是否在工作区范围内
 * @param te 
 * @param ofs 
 * @return true 
 * @return false 
 */
bool te_is_begin_of_ws(te_t te, te_ofs_t ofs)
{
    TE_UNUSED(te);
    return (ofs <= 0);
}

/**
 * @brief 检查 ofs 是否在工作区范围内
 * @param te 
 * @param ofs 
 * @return true 
 * @return false 
 */
bool te_is_end_of_ws(te_t te, te_ofs_t ofs)
{
    return (ofs > (te_ws_len(te) - 1));
}

/**
 * @brief 统计在工作区的 [start, stop] 范围内指定字符的个数
 * @param te 
 * @param start 
 * @param stop 
 * @param c 
 * @return int 
 */
int te_get_char_count(te_t te, te_ofs_t start, te_ofs_t stop, char c)
{
    if(!te_is_ofs_valid(te, start))
        return -1;
    if(!te_is_ofs_valid(te, stop))
        return -1;

    int count = 0;
    start = (te_is_begin_of_ws(te, start)) ? 0 : start;
    stop = (!te_is_ofs_valid(te, stop)) ? (te_ofs_t)te_ws_used(te) : stop;
    for(te_ofs_t i = start; i < stop; i++)
        if(te_ws_buf(te)[i] == c)
            count++;
    return count;
}

/**
 * @brief 正向查找第一次出现目标字符的偏移量
 * @param te
 * @param start
 * @param stop
 * @param c
 * @return
 */
te_ofs_t te_find_char(te_t te, te_ofs_t start, te_ofs_t stop, char c)
{
    if(!te_is_ofs_valid(te, start))
        return -1;
    if(!te_is_ofs_valid(te, stop))
        return -1;
    if(start >= stop)
        return -1;

    for(te_ofs_t ofs = start; ofs <= stop; ++ofs)
        if(te_ws_buf_at(te, ofs) == c)
            return ofs;
    return -1;
}

/**
 * @brief 逆向查找第一次出现目标字符的偏移量
 * @param te
 * @param start
 * @param stop
 * @param c
 * @return
 */
te_ofs_t te_r_find_char(te_t te, te_ofs_t start, te_ofs_t stop, char c)
{
    if(!te_is_ofs_valid(te, start))
        return -1;
    if(!te_is_ofs_valid(te, stop))
        return -1;
    if(start <= stop)
        return -1;

    for(te_ofs_t ofs = start; ofs >= stop; --ofs)
        if(te_ws_buf_at(te, ofs) == c)
            return ofs;
    return -1;
}

/**
 * @brief 正向查找不为目标字符时出现的第一个字符的偏移量
 * @param te
 * @param start
 * @param stop
 * @param c
 * @return
 */
te_ofs_t te_ignore_char(te_t te, te_ofs_t start, te_ofs_t stop, char c)
{
    if(!te_is_ofs_valid(te, start))
        return -1;
    if(!te_is_ofs_valid(te, stop))
        return -1;
    if(start >= stop)
        return -1;

    for(te_ofs_t ofs = start; ofs <= stop; ++ofs)
        if(te_ws_buf_at(te, ofs) != c)
            return ofs;
    return 0;
}

/**
 * @brief 逆向查找不为目标字符时出现的第一个字符的偏移量
 * @param te
 * @param start
 * @param stop
 * @param c
 * @return
 */
te_ofs_t te_r_ignore_char(te_t te, te_ofs_t start, te_ofs_t stop, char c)
{
    if(!te_is_ofs_valid(te, start))
        return -1;
    if(!te_is_ofs_valid(te, stop))
        return -1;
    if(start <= stop)
        return -1;

    for(te_ofs_t ofs = start; ofs >= stop; --ofs)
        if(te_ws_buf_at(te, ofs) != c)
            return ofs;
    return -1;
}

/**
 * @brief 获取 ofs 所在行的行首在 工作区 中的偏移量
 * @param te 
 * @param ofs 
 * @return te_ofs_t 
 */
te_ofs_t te_get_line_start(te_t te, te_ofs_t ofs)
{
    if(!te_is_ofs_valid(te, ofs))
        return -1;

    for(te_ofs_t i = ofs - 1; i >= 0; --i)
    {
        if(ofs == 0)
            return 0;
        if(te_ws_buf(te)[i] == '\n')
            return i + 1;     
    }
    return 0;
}

/**
 * @brief 计算 ofs 所在行的行尾在 工作区 中的偏移量
 * @note 该函数将 '\n' 和 '\0' 视为行的一部分，作为行尾标志
 * @param te 
 * @param ofs 
 * @return te_ofs_t 
 */
te_ofs_t te_get_line_end(te_t te, te_ofs_t ofs)
{
    if(!te_is_ofs_valid(te, ofs))
        return -1;

    for (te_ofs_t i = ofs; i < te_ws_used(te); i++)
    {
        switch(te_ws_buf(te)[i])
        {
        case '\n':
        case '\0':
            return i;
        }
    }
    return te_ws_used(te) - 1;
}           

/**
 * @brief 获取 ofs 所在行的长度
 * @note 该函数计算的长度包含 '\n' 和 '\0'
 * @example 
 *          ofs:    01234 5
 *          str:    hello[\0]
 *          start: 0, end: 5
 *          len: end - start + 1 = 6
 * @param te 
 * @param ofs 
 * @return int 
 */
int te_get_line_len(te_t te, te_ofs_t ofs)
{
    if(!te_is_ofs_valid(te, ofs))
        return -1;
    te_ofs_t start = te_get_line_start(te, ofs);
    te_ofs_t end = te_get_line_end(te, ofs);
    return end - start + 1;     // 包含换行符
}          

/**
 * @brief 计算 ofs 所在行的上一行行首在 工作区 中的偏移量
 * @param te 
 * @param ofs 
 * @return te_ofs_t 
 */
te_ofs_t te_get_prev_line_start(te_t te, te_ofs_t ofs)
{
    if(!te_is_ofs_valid(te, ofs))
        return -1;
    te_ofs_t start = te_get_line_start(te, ofs);
    if(te_is_begin_of_ws(te, start))
        return -1;
    ofs = start - 1;
    return te_get_line_start(te, ofs);
}         

/**
 * @brief 计算 ofs 所在行的下一行行首在 工作区 中的偏移量
 * @param te 
 * @param ofs 
 * @return te_ofs_t 
 */
te_ofs_t te_get_next_line_start(te_t te, te_ofs_t ofs)
{
    if(!te_is_ofs_valid(te, ofs))
        return -1;
    te_ofs_t end = te_get_line_end(te, ofs);
    if(!te_is_ofs_valid(te, end + 1) || te_ws_buf_at(te, end) == '\0')
        return -1;
    else
        return end + 1;
}         

/**
 * @brief 获取 ofs 所在行的行号 
 * @param te 
 * @param ofs 
 * @return int 
 */
int te_get_cur_line(te_t te, te_ofs_t ofs)
{
    return te_get_char_count(te, 0, ofs, '\n') + 1;
}

/**
 * @brief 获取 ofs 所在列的列号 
 * @param te 
 * @param ofs 
 * @return int 
 */
int te_get_cur_col(te_t te, te_ofs_t ofs)
{
    if(!te_is_ofs_valid(te, ofs))
        return -1;
    else
        return ofs - te_get_line_start(te, ofs) + 1;
}    

/**
 * @brief 获取 line 的行首在 工作区 中的偏移量
 * @param te 
 * @param line 
 * @return te_ofs_t 
 */
te_ofs_t te_get_line_ofs(te_t te, int line)
{
    if(line == 1)
        return 0;

    int lnbr = 1;
    for(te_ofs_t i = 0; i < te_ws_used(te); ++i)
        if(te_ws_buf(te)[i] == '\n')
            if(++lnbr == line)
                return i + 1;
    return -1;      // 行号不存在
}

/**
 * @brief 获取指定行号的值的位数
 * @param te 
 * @param nbr 
 * @return int 
 */
int te_get_line_nbr_digits(te_t te, int nbr)
{
    TE_UNUSED(te);
    int count = 0;
    do
    {
        count++;
        nbr /= 10;
    } while (nbr != 0);
    return count;
}

/**
 * @brief 通过偏移量获取光标在工作区的绝对行号 
 * @param te 
 * @param ofs 
 * @return int 
 */
int te_get_cur_abs_line(te_t te, te_ofs_t ofs)
{
    return te_get_cur_line(te, ofs);
}

/**
 * @brief 通过偏移量获取光标在工作区的相对行位置
 * @param te 
 * @param ofs 
 * @return int 
 */
int te_get_cur_rel_line(te_t te, te_ofs_t ofs)
{
    int line = te_get_cur_abs_line(te, ofs);
    return line;
}

/**
 * @brief 通过绝对偏移量获取光标在工作区的绝对列位置 
 * @param te 
 * @param ofs 
 * @return int 
 */
int te_get_cur_abs_col(te_t te, te_ofs_t ofs)
{
#define SPACE_LEN_TO_WORKSPACE 2

    if(!te_is_ofs_valid(te, ofs))
        return -1;

    int max     = te_ws_lines(te);                      // 获取最大行号
    int digits  = te_get_line_nbr_digits(te, max);      // 获取行号位数，位数决定了行号所占用的宽度
    int col     = te_get_cur_col(te, ofs);
    return digits + SPACE_LEN_TO_WORKSPACE + col;

#undef SPACE_LEN_TO_WORKSPACE
}

/**
 * @brief 通过偏移量获取光标在工作区的相对列位置
 * @param te 
 * @param ofs 
 * @return int 
 */
int te_get_cur_rel_col(te_t te, te_ofs_t ofs)
{
    return te_get_cur_col(te, ofs);
}

/**
 * @brief 获取上一个单词在工作区的偏移量
 * @example
 *          hello w|orld
 * @param te 
 * @param ofs 
 * @return te_ofs_t 
 */
te_ofs_t te_prev_word(te_t te, te_ofs_t ofs)
{
    if(!te_is_ofs_valid(te, ofs))
        return -1;
    te_ofs_t space_ofs = te_r_find_char(te, ofs, 0, ' ');
    if(space_ofs > 0)
    {
        te_ofs_t alpha_ofs = te_r_ignore_char(te, space_ofs - 1, 0, ' ');
        if(alpha_ofs > 0)
        {
            te_ofs_t prev_space_ofs = te_r_find_char(te, alpha_ofs, 0, ' ');
            if(prev_space_ofs > 0)
                return te_ignore_char(te, prev_space_ofs + 1, ofs, '\n');
            else
                return 0;
        }
        else
            return te_ignore_char(te, space_ofs + 1, ofs, '\n');
    }
    return 0;
}

/**
 * @brief 获取下一个单词在工作区的偏移量
 * @param te 
 * @param ofs 
 * @return te_ofs_t 
 */
te_ofs_t te_next_word(te_t te, te_ofs_t ofs)
{
    if(!te_is_ofs_valid(te, ofs))
        return -1;
    te_ofs_t res = te_find_char(te, ofs, te_ws_used(te) - 1, ' ');
    if(res < 0)
        return -1;
    else
    {
        res = te_ignore_char(te, res, te_ws_used(te) - 1, ' ');
        if(te_is_ofs_valid(te, res))
            if(te_ws_buf_at(te, res) != '\0')
                return te_ignore_char(te, res, te_ws_used(te) - 1, '\n');
    }
    return -1;
}



















