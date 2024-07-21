/**
 * @file te_keys_for_cmd.c
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

static int _curr_char_delete(te_t te)
{
    te_del_str_on_disp(te, te_ws_ofs(te), NULL, 1);
    return 0;
}

static int _line_feed(te_t te)
{
    TE_UNUSED(te);
    return 0;
}

static int _into_insmod(te_t te)
{
    te_set_mode(te, Te_Mode_Insert);
    te_char_clear(te, &te->cmd);
    te_char_clear(te, &te->input);
    return 0;
}

static int _into_insmod_with_new_line(te_t te)
{
    _into_insmod(te);
    te_ofs_t end = te_get_line_end(te, te_ws_ofs(te));      // 获取光标所在行的行尾
    if(te_is_ofs_valid(te, end))                            // 检查偏移量是否有效
    {
        te_reset_cur(te, end);                              // 将光标重置到行尾，此时的偏移量将重定向到行尾。
        te_ins_str_on_disp(te, te_ws_ofs(te), "\n", 1);     // 插入换行符
    }
    return 0;
}

static int _into_llmod(te_t te)
{
    te_set_mode(te, Te_Mode_Last_Line);
    te_char_clear(te, &te->cmd);
    te_char_push(te, &te->cmd, te_input_buf_at(te, 0));
    return 0;
}

static int _goto_line_start(te_t te)
{
    te_ofs_t start = te_get_line_start(te, te_ws_ofs(te));
    te_reset_cur(te, start);
    return 0;
}

static int _goto_line_end(te_t te)
{
    te_ofs_t end = te_get_line_end(te, te_ws_ofs(te));
    te_reset_cur(te, end);
    return 0;
}

TE_UNUSED_FUNC static int _goto_prev_word(te_t te)
{
    te_ofs_t ofs = te_prev_word(te, te_ws_ofs(te) - 1);
    if(te_is_ofs_valid(te, ofs))
        te_reset_cur(te, ofs);
    return 0;
}

TE_UNUSED_FUNC static int _goto_next_word(te_t te)
{
    te_ofs_t ofs = te_next_word(te, te_ws_ofs(te));
    if(te_is_ofs_valid(te, ofs))
        te_reset_cur(te, ofs);
    return 0;
}

static int _change_case(te_t te)
{
    te_ofs_t ofs = te_ws_ofs(te);
    char c = te_ws_buf_at(te, ofs);
    if(isalpha(c))
    {
        if(isupper(c))
            te_ws_buf_at(te, ofs) = tolower(c);
        if(islower(c))
            te_ws_buf_at(te, ofs) = toupper(c);
        te_refr_ws_on_disp(te, ofs, 1, TE_RDF_CUR_TO_ORIGIN);
    }
    return 0;
}

static int _copy_line(te_t te)
{
    te_ofs_t start = te_get_line_start(te, te_ws_ofs(te));
    int len = te_get_line_len(te, start) - 1;       // 不包含 '\n' 和 '\0'
    len = (len > te_clip_len(te)) ? te_clip_len(te) : len;

    /** 清空剪贴板 **/
    te_char_clear(te, &te->clip);

    /** 复制到剪贴板 **/
    for(int i = 0; i < len; ++i)
        te_char_push(te, &te->clip, te_ws_buf_at(te, start + i));

    return 0;
}

static int _clip_line(te_t te)
{
    _copy_line(te);
    te_ofs_t start = te_get_line_start(te, te_ws_ofs(te));
    te_ofs_t end = te_get_line_end(te, te_ws_ofs(te)); 
    int len = te_clip_used(te);
    if(te_ws_buf_at(te, end) == '\n')   // 检查原有内容是否包含 '\n', 
        ++len;                          // 如果包含，则也要 '\n' 其去除,
                                        // 因为 clip_line 剪切的是整行，因此在 ws[end] 一定是 '\n' 或 '\0'.
    te_del_str_on_disp(te, start, NULL, len);
    return 0;
}

static int _paste_below(te_t te)
{
    if(te_clip_used(te) <= 0)
        return -1;
    te_ofs_t end = te_get_line_end(te, te_ws_ofs(te));      // 查找行尾
    if(te_is_ofs_valid(te, end))                            // 检查偏移量是否有效
    {
        te_reset_cur(te, end);                              // 将光标重置到行尾，此时的偏移量将重定向到行尾。
        te_ins_str_on_disp(te, te_ws_ofs(te), "\n", 1);     // 然后在行尾插入 '\n'，例如："hello[\0]"，其中 [\0] 为行尾，插入 '\n' 后变成 "hello[\n][\0]"，
                                                            // te_ins_str_at_ws() 插入时，会将原位置的内容后置，因此插入 '\n' 后，[\n] 会在 [\0] 前面。
        te_ins_str_on_disp(te, te_ws_ofs(te), te_clip_buf(te), te_clip_used(te));       // 插入后："hello[\n]<新插入内容>[\0]"，此时 ws[te::ofs] = '\0'
    }
    return 0;
}










/**
 * @brief 功能组合键集
 */
static struct te_key cmdmod_keys[] =
{
    /** 与光标移动有关 **/
    {.key = "\x00\x48",     .len = 2, .fn = _cur_up},               // 光标上移
    {.key = "\xe0\x48",     .len = 2, .fn = _cur_up},               // 光标上移
    {.key = "j",            .len = 1, .fn = _cur_up},               // 光标上移
    {.key = "\033[A",       .len = 3, .fn = _cur_up},               // 光标上移
    {.key = "\x00\x50",     .len = 2, .fn = _cur_down},             // 光标下移
    {.key = "\xe0\x50",     .len = 2, .fn = _cur_down},             // 光标下移
    {.key = "k",            .len = 1, .fn = _cur_down},             // 光标下移
    {.key = "\033[B",       .len = 3, .fn = _cur_down},             // 光标下移
    {.key = "\x00\x4b",     .len = 2, .fn = _cur_left},             // 光标左移
    {.key = "\xe0\x4b",     .len = 2, .fn = _cur_left},             // 光标左移
    {.key = "h",            .len = 1, .fn = _cur_left},             // 光标左移
    {.key = "\b",           .len = 1, .fn = _cur_left},             // 光标左移
    {.key = "\033[D",       .len = 3, .fn = _cur_left},             // 光标左移
    {.key = "\x7f",         .len = 1, .fn = _cur_left},             // 光标左移
    {.key = "\x00\x4d",     .len = 2, .fn = _cur_right},            // 光标右移
    {.key = "\xe0\x4d",     .len = 2, .fn = _cur_right},            // 光标右移
    {.key = "l",            .len = 1, .fn = _cur_right},            // 光标右移
    {.key = "\033[C",       .len = 3, .fn = _cur_right},            // 光标右移

    /** 无操作组合键 **/
    {.key = "\r",           .len = 1, .fn = _line_feed},            // 换行
    {.key = "\n",           .len = 1, .fn = _line_feed},            // 换行

    /** 模式进入 **/
    {.key = "i",            .len = 1, .fn = _into_insmod},                  // 进入 insert 模式
    {.key = "o",            .len = 1, .fn = _into_insmod_with_new_line},    // 以插入新行的方式进入 insert 模式
    {.key = ":",            .len = 1, .fn = _into_llmod},                   // 进入 last line 模式
    {.key = "/",            .len = 1, .fn = _into_llmod},                   // 进入 last line 模式

    /** 功能命令 **/
    {.key = "0",            .len = 1, .fn = _goto_line_start},      // 移动光标到当前行首
    {.key = "$",            .len = 1, .fn = _goto_line_end},        // 移动光标到当前行尾
//    {.key = "b",            .len = 1, .fn = _goto_prev_word},       // 移动光标到上一个单词的首字母，忽略大小写和标点符号
//    {.key = "B",            .len = 1, .fn = _goto_prev_word},       // 移动光标到上一个单词的首字母，忽略大小写和标点符号
//    {.key = "w",            .len = 1, .fn = _goto_next_word},       // 移动光标到下一个单词的首字母，忽略大小写和标点符号
//    {.key = "W",            .len = 1, .fn = _goto_next_word},       // 移动光标到下一个单词的首字母，忽略大小写和标点符号
    {.key = "x",            .len = 1, .fn = _prev_char_delete},     // 删除光标所在的一个字符
    {.key = "X",            .len = 1, .fn = _curr_char_delete},     // 删除光标前一个字符
    {.key = "~",            .len = 1, .fn = _change_case},          // 大小写切换
    {.key = "yy",           .len = 2, .fn = _copy_line},            // 复制光标所在行
    {.key = "dd",           .len = 2, .fn = _clip_line},            // 剪切光标所在行
    {.key = "p",            .len = 1, .fn = _paste_below},          // 粘贴剪贴板内容到当前行的下一行

    {.key = NULL, .len = 0, .fn = NULL},
};



te_key_t te_get_def_cmdmod_keys(void)
{
    return cmdmod_keys;
}
