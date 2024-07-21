/**
 * @file te_keys_for_insert.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-07-16
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "texteditor.h"

static int _cur_up(te_t te)
{
    te_cur_up(te, 1);
    return 0;
}

static int _cur_down(te_t te)
{
    te_cur_down(te, 1);
    return 0;
}

static int _cur_left(te_t te)
{
    te_cur_left(te, 1);
    return 0;
}

static int _cur_right(te_t te)
{
    te_cur_right(te, 1);
    return 0;
}

static int _prev_char_delete(te_t te)
{
    // 因为减去的字符实际是光标所在位置的前一个字符，其对应到是 ws[ofs-1] 位置，所以需要 -1
    te_del_str_on_disp(te, te_ws_ofs(te) - 1, NULL, 1);     
    return 0;
}

static int _line_feed(te_t te)
{
    te_ins_str_on_disp(te, te_ws_ofs(te), "\n", 1);
    return 0;
}

static int _esc_insert_mode(te_t te)
{
    te_set_mode(te, Te_Mode_Cmd);
    return 0;
}

static int _hor_tab(te_t te)
{
    // 水平制表符用空格填充，填充的空格数最长为 4
    int n = 4 - ((te_get_cur_col(te, te_ws_ofs(te)) - 1) % 4);
    const char* buf = "    ";
    te_ins_str_on_disp(te, te_ws_ofs(te), buf, n);
    return 0;
}



/**
 * @brief 功能组合键集
 */
static struct te_key insmod_keys[] =
{
    /** 光标移动 **/
    {.key = "\x00\x48",     .len = 2, .fn = _cur_up},         // 光标上移
    {.key = "\xe0\x48",     .len = 2, .fn = _cur_up},         // 光标上移
    {.key = "\033[A",       .len = 3, .fn = _cur_up},         // 光标上移
    {.key = "\x00\x50",     .len = 2, .fn = _cur_down},       // 光标下移
    {.key = "\xe0\x50",     .len = 2, .fn = _cur_down},       // 光标下移
    {.key = "\033[B",       .len = 3, .fn = _cur_down},       // 光标下移
    {.key = "\x00\x4b",     .len = 2, .fn = _cur_left},       // 光标左移
    {.key = "\xe0\x4b",     .len = 2, .fn = _cur_left},       // 光标左移
    {.key = "\033[D",       .len = 3, .fn = _cur_left},       // 光标左移
    {.key = "\x00\x4d",     .len = 2, .fn = _cur_right},      // 光标右移
    {.key = "\xe0\x4d",     .len = 2, .fn = _cur_right},      // 光标右移
    {.key = "\033[C",       .len = 3, .fn = _cur_right},      // 光标右移

    /** 字符增删 **/
    {.key = "\b",           .len = 1, .fn = _prev_char_delete},     // 移除一个字符
    {.key = "\x7f",         .len = 1, .fn = _prev_char_delete},     // 移除一个字符
    {.key = "\r",           .len = 1, .fn = _line_feed},            // 换行
    {.key = "\n",           .len = 1, .fn = _line_feed},            // 换行
    {.key = "\t",           .len = 1, .fn = _hor_tab},              // 插入水平制表符

    /** 退出 insert 模式 **/
    {.key = "\033\033",     .len = 2, .fn = _esc_insert_mode},      // 退出 insert 模式

    {.key = NULL, .len = 0, .fn = NULL},
};



te_key_t te_get_def_insmod_keys(void)
{
    return insmod_keys;
}
