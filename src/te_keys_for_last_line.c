/**
 * @file te_keys_for_last_line.c
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
    TE_UNUSED(te);
    return 0;
}

static int _cur_down(te_t te)
{
    TE_UNUSED(te);
    return 0;
}

static int _cur_left(te_t te)
{
    TE_UNUSED(te);
    return 0;
}

static int _cur_right(te_t te)
{
    TE_UNUSED(te);
    return 0;
}

static int _char_delete(te_t te)
{
    te_char_pop(te, &te->cmd);
    return 0;
}

static int _str_match(te_t te)
{
    // 清屏
    te_refr_ws_on_disp(te, 0, te_ws_used(te), TE_RDF_CUR_TO_ORIGIN);

    // 获取参数
    char* target = te_cmd_buf(te) + 1;
    int len = strlen(target);
    if(!len)
        return -1;
    char* pos = te_ws_buf(te);


    // 对匹配成功的内容进行着色
    te_plt_set(te, TX_BLACK BK_WHITE, Te_Plt_Scope_String_Match);
    while(1)
    {
        char* match = strstr(pos, target);
        if(match == NULL)
            break;
        te_ofs_t ofs = match - te_ws_buf(te);
        te_refr_ws_on_disp(te, ofs, len, TE_RDF_CUR_TO_ORIGIN);
        pos++;
    }
    te_plt_disable(te);

    return 0;
}

static int _entry(te_t te)
{
#define SAVE_FILE       (1 << 0)
#define QUIT_EDITOR     (1 << 1)

    int flag = 0;
    char* arg = te_cmd_buf(te);
    const char* path = te_file_name(te);

    for(int i = 0; i < te_cmd_len(te); i++)
    {
        switch(arg[i])
        {
        case 'w': flag |= SAVE_FILE; break;         // 保持工作区内容到文件中
        case 'q': flag |= QUIT_EDITOR; break;       // 退出编辑器
        case ' ':                                   // 路径参数
            if(isalnum(arg[i + 1]))
            {
                int len = strlen(arg + i + 1);
                path = arg + i + 1;
                i += len + 1;
            } break;

        case '/':       // 查询
        case '?':       // 查询
            if(i == 0)
                return _str_match(te);      // 进行字符串匹配
            break;

        case 'l':       // 显示行号
            if(i == 1)
            {
                te_set_flag(te, TE_FLAG_LINE_NBR_SHOW);                             // 设置标志位
                te_printf(te, CLS);
                te_refr_ws_on_disp(te, 0, te_ws_used(te), TE_RDF_CUR_TO_ORIGIN);    // 强制重绘
                te_set_mode(te, Te_Mode_Cmd);                                       // 强制退出底行模式
                te_char_clear(te, &te->cmd);
                return 0;
            } break;

        case 'L':       // 隐藏行号 
            if(i == 1)
            {
                te_clr_flag(te, TE_FLAG_LINE_NBR_SHOW);                             // 清空标志位
                te_printf(te, CLS);
                te_refr_ws_on_disp(te, 0, te_ws_used(te), TE_RDF_CUR_TO_ORIGIN);    // 强制重绘
                te_set_mode(te, Te_Mode_Cmd);                                       // 强制退出底行模式
                te_char_clear(te, &te->cmd);
                return 0;
            } break;
        }
    }
    
    if(flag & SAVE_FILE)
        te_file_sync(te, path);
    if(flag & QUIT_EDITOR)
        te_quit(te);

#undef SAVE_FILE
#undef QUIT_EDITOR

    return 0;
}

static int _hor_tab(te_t te)
{
    TE_UNUSED(te);
    return 0;
}

static int _esc_ll_mode(te_t te)
{
    te_set_mode(te, Te_Mode_Cmd);
    te_char_clear(te, &te->cmd);
    return 0;
}


/**
 * @brief 功能组合键集
 */
static struct te_key llmod_keys[] =
{
    /** 光标移动 **/
    {.key = "\x00\x48",     .len = 2, .fn = _cur_up},               // 光标上移
    {.key = "\xe0\x48",     .len = 2, .fn = _cur_up},               // 光标上移
    {.key = "\x00\x50",     .len = 2, .fn = _cur_down},             // 光标下移
    {.key = "\xe0\x50",     .len = 2, .fn = _cur_down},             // 光标下移
    {.key = "\x00\x4b",     .len = 2, .fn = _cur_left},             // 光标左移
    {.key = "\xe0\x4b",     .len = 2, .fn = _cur_left},             // 光标左移
    {.key = "\x00\x4d",     .len = 2, .fn = _cur_right},            // 光标右移
    {.key = "\xe0\x4d",     .len = 2, .fn = _cur_right},            // 光标右移

    /** 字符增删 **/
    {.key = "\b",           .len = 1, .fn = _char_delete},          // 移除一个字符
    {.key = "\x7f",         .len = 1, .fn = _char_delete},          // 移除一个字符

    /** 无操作 **/
    {.key = "\r",           .len = 1, .fn = _entry},                // 执行指令解析
    {.key = "\n",           .len = 1, .fn = _entry},                // 执行指令解析
    {.key = "\t",           .len = 1, .fn = _hor_tab},              // 水平制表符

    /** 特殊功能 **/
//    {.key = "/",            .len = 1, .fn = _str_match},            // 字符串匹配
//    {.key = "?",            .len = 1, .fn = _str_match},            // 字符串匹配

    /** 退出 last line 模式 */
    {.key = "\033",         .len = 1, .fn = _esc_ll_mode},          // 退出 last line 模式

    {.key = NULL, .len = 0, .fn = NULL},
};




te_key_t te_get_def_llmod_keys(void)
{
    return llmod_keys;
}
