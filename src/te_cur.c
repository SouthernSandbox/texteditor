/**
 * @file te_cur.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-07-20
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "texteditor.h"

/**
 * @brief 将当前光标上移 n 行
 * @param te 
 * @param n
 * @return te_ofs_t 
 */
te_ofs_t te_cur_up(te_t te, int n)
{
    for(int i = 0; i < n; ++i)
    {
        int pre_start = te_get_prev_line_start(te, te_ws_ofs(te));
        if(pre_start < 0)
            break;      // 已经是第一行了

        int pre_len = te_get_line_len(te, pre_start);       // 获得上一行的长度
        int col = te_get_cur_col(te, te_ws_ofs(te));        // 获得光标所在当前行的列数
        col = (col > pre_len) ? pre_len : col;              // 防止超出上一行的长度
        te_reset_cur(te, pre_start + (col - 1));            // 重置光标位置，由于 col 是从 1 开始的，所以需要减 1
    }

    return te_ws_ofs(te);
}

/**
 * @brief 将当前光标下移 n 行
 * @param te 
 * @param n
 * @return te_ofs_t 
 */
te_ofs_t te_cur_down(te_t te, int n)
{
    for(int i = 0; i < n; ++i)
    {
        int next_start = te_get_next_line_start(te, te_ws_ofs(te));
        if(next_start < 0)
            break;      // 已经是最后一行了

        int next_len = te_get_line_len(te, next_start);     // 获得下一行的长度
        int col = te_get_cur_col(te, te_ws_ofs(te));        // 获得光标所在当前行的列数
        col = (col > next_len) ? next_len : col;            // 防止超出下一行的长度
        te_reset_cur(te, next_start + (col - 1));           // 重置光标位置，由于 col 是从 1 开始的，所以需要减 1
    }

    return te_ws_ofs(te);
}

/**
 * @brief 将当前光标左移 n 列
 * @param te 
 * @param n
 * @return te_ofs_t 
 */
te_ofs_t te_cur_left(te_t te, int n)
{
    int start = te_get_line_start(te, te_ws_ofs(te));

    for(int i = 0; i < n; ++i)
        if(te_ws_ofs(te) > start)
        {
            te_printf(te, CUB(1));
            te_ws_ofs(te)--;
        }

    return te_ws_ofs(te);
}

/**
 * @brief 将当前光标右移 n 列
 * @param te 
 * @param n
 * @return te_ofs_t 
 */
te_ofs_t te_cur_right(te_t te, int n)
{
    int end = te_get_line_end(te, te_ws_ofs(te));
    if(!te_is_ofs_valid(te, end))
        return -1;

    for(int i = 0; i < n; ++i)
        if(te_ws_ofs(te) < end)
        {
            te_printf(te, CUF(1));
            te_ws_ofs(te)++;
        }

    return te_ws_ofs(te);
}

/**
 * @brief 将光标移动至指定行列
 * @param te 
 * @param line 
 * @param row 
 * @return te_ofs_t 
 */
te_ofs_t te_cur_move_to(te_t te, int line, int row)
{
    if(line <= 0)
        line = 1;
    if(row <= 0)
        row = 1;
    te_ofs_t ofs = te_get_line_ofs(te, line) + (row - 1);
    if(!te_is_ofs_valid(te, ofs))
        return -1;
    te_reset_cur(te, ofs);
    return te_ws_ofs(te);
}

/**
 * @brief 将光标重置到 HOME 位置
 * @param te 
 * @return te_ofs_t 
 */
te_ofs_t te_cur_back_home(te_t te)
{
    te_reset_cur(te, 0);
    return te_ws_ofs(te);
}

/**
 * @brief 将光标重置到行首
 * @param te
 * @return
 */
te_ofs_t te_cur_move_to_line_head(te_t te)
{
    int line = te_get_cur_line(te, te_ws_ofs(te));
    te_cur_move_to(te, line, 1);
    return te_ws_ofs(te);
}

/**
 * @brief 将光标重置到行尾
 * @param te
 * @return
 */
te_ofs_t te_cur_move_to_line_tail(te_t te)
{
    int line = te_get_cur_line(te, te_ws_ofs(te));
    int end = te_get_line_end(te, te_ws_ofs(te));
    te_cur_move_to(te, line, end + 1);
    return te_ws_ofs(te);
}

/**
 * @brief 依据指定的偏移量，重置光标的位置
 * @note 这个函数会重置编辑器的工作区偏移量
 * @param te
 * @param ofs
 * @return int
 */
int te_reset_cur(te_t te, te_ofs_t ofs)
{
    if(!te_is_ofs_valid(te, ofs))
        return -1;
    int ln = te_get_cur_line(te, ofs);
    int col = te_get_cur_col(te, ofs);
    if(te_tst_flag(te, TE_FLAG_LINE_NBR_SHOW))
        te_printf(te, CUP(ln, col + TE_LINE_NUMBER_WIDTH));
    else 
        te_printf(te, CUP(ln, col));
    te_ws_ofs(te) = ofs;
    return 0;
}
