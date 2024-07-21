/**
 * @file te_core.c
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
 * @brief 打印插入模式日志
 * @param te
 * @return int
 */
static int _insert_mode_log(te_t te)
{
    te_printf(te, CU_HIDE);
    te_printf(te, SC);
    te_printf(te, CUP(999, 0));
    te_printf(te, CUU(1));
    te_printf(te, CLL);
    te_printf(te, "workspace(used: %d, ofs: %d, ln: %d, col: %d, lmax: %d): ",
                    te_ws_used(te),
                    te_ws_ofs(te),
                    te_get_cur_line(te, te_ws_ofs(te)),
                    te_get_cur_col(te, te_ws_ofs(te)),
                    te_ws_lines_max(te));

    for(int i = 0; i < te_ws_used(te); ++i)
    {
        char c = te_ws_buf_at(te, i);
        if(isprint(c))
            te_putchar(te, c);
        else
        {
            te_printf(te, TX_GREEN);
            switch(c)
            {
            case '\n':  te_printf(te, "[\\n]"); break;
            case '\0':  te_printf(te, "[\\0]"); break;
            default:    te_printf(te, "[\\?]"); break;;
            }
            te_printf(te, TX_DEF);
        }
    }
    te_printf(te, RC);
    te_printf(te, CU_SHOW);
    return 0;
}





/**
 * @brief 尝试执行组合键功能
 * @param te 
 * @param keys 
 * @return int 
 */
static int _try_exec_keys(te_t te, te_key_t keys, const char* key)
{
    te_clr_flag(te, TE_FLAG_CATCH_KEY_HEAD);    // 清空标志位

    /** 遍历注册组合键 **/
    for(int i = 0; keys[i].key != NULL; ++i)
    {
        /** 组合键匹配 **/
        if(memcmp(key, keys[i].key, keys[i].len) == 0)
        {
            keys[i].fn(te);
            te_char_clear(te, &te->input);              // 清空缓冲区
            te_clr_flag(te, TE_FLAG_CATCH_KEY_HEAD);    // 清空标志位
            return 0;
        }
        else
        {
            /** 匹配失败，检查是否存在组合键头 **/
            if(memcmp(key, keys[i].key, strlen(key)) == 0)
                te_set_flag(te, TE_FLAG_CATCH_KEY_HEAD);        // 如果有部分内容能成功匹配，则设置标志位
        }
    }

    /** 如果输入的内容在注册的组合键中不存在，则清空输入 **/
    if(!te_tst_flag(te, TE_FLAG_CATCH_KEY_HEAD))
        te_char_clear(te, &te->input);

    return 0;
}

/**
 * @brief 绘制行数
 * @param te 
 * @return int 
 */
static int _draw_line_nbr(te_t te)
{
    if(!te_tst_flag(te, TE_FLAG_LINE_NBR_SHOW))
        return -1;

    te_printf(te, CU_HIDE);         // 隐藏光标
    te_printf(te, SC);
    int curr_line = te_get_cur_line(te, te_ws_ofs(te));     // 获取当前行号
    for(int line = 1; line <= te_ws_lines_max(te); line++)
    {
        te_printf(te, CUP(line, 1));        // 移动光标
        if(line <= te_ws_lines(te))
        {
            if(line != curr_line)
                te_plt_use(te, Te_Plt_Scope_Line_Nbr);          // 普通行号着色
            else
                te_plt_use(te, Te_Plt_Scope_Line_Nbr_Curr);     // 当前行号着色

            te_printf(te, "%3d", line);     // 绘制行号
        }
        else 
        {
            te_plt_disable(te);             // 停用调色板
            te_printf(te, "   ");           // 清空行首到光标之间的内容
        }
    }
    te_plt_disable(te);         // 停用调色板
    te_printf(te, RC);
    te_printf(te, CU_SHOW);     // 显示光标
    return 0;
}

/**
 * @brief 擦除底部状态栏信息
 * @param te 
 * @return int 
 */
static int _erase_status_bar_info(te_t te)
{
    te_printf(te, CU_HIDE);
    te_printf(te, SC);
    te_printf(te, CUP(999, 0));
    te_printf(te, CLL);
    te_printf(te, RC);
    te_printf(te, CU_SHOW);
    return 0;
}

/**
 * @brief 绘制底部状态栏信息
 * @param te 
 * @return int 
 */
static int _draw_status_bar_info(te_t te)
{
    /** 隐藏光标，避免出现闪烁干扰 **/
    te_printf(te, CU_HIDE);

    /** 保存并移动光标，擦除底部状态栏信息 **/
    te_printf(te, SC);
    te_printf(te, CUP(999, 0));
    te_printf(te, CLL);

    /** 模式名称 **/
    const char* mode_names[] = 
    {
        [Te_Mode_Cmd]       = "CMD",
        [Te_Mode_Last_Line] = "LLN",
        [Te_Mode_Insert]    = "INS",
    };

    /** 打印状态栏信息 **/
    te_plt_use(te, Te_Plt_Scope_Status_Bar);
    te_printf( te, 
                "[ - %3s - ] %s ",
                mode_names[te->mode],
                (te_file_name(te)) ? (te_file_name(te)) : "");
    te_plt_disable(te);

    // 依据不同模式打印补充信息
    te_plt_use(te, Te_Plt_Scope_Status_Bar_Sup);
    switch(te->mode)
    {
    case Te_Mode_Last_Line:
        te_printf(te, "%s", te_cmd_buf(te));
        break;

    case Te_Mode_Cmd:
        {
            char c = te_ws_buf_at(te, te_ws_ofs(te));
            te_printf( te,
                        " %2c (%d,%d) ",
                        isprint(c) ? c : ' ',
                        te_get_cur_line(te, te_ws_ofs(te)),
                        te_get_cur_col(te, te_ws_ofs(te)));

            for(int i = 0; i < te_input_used(te); i++)
            {
                char c = te_input_buf_at(te, i);
                if(isprint(c))
                    te_putchar(te, c);
                else
                {
                    te_plt_use_temp(te, TX_GREEN);
                    switch(c)
                    {
                    case '\t':      te_printf(te, "[\\t]"); break;
                    case '\n':      te_printf(te, "[\\n]"); break;
                    case '\b':      te_printf(te, "[\\b]"); break;
                    case '\r':      te_printf(te, "[\\r]"); break;
                    case '\033':    te_printf(te, "[\\033]"); break;
                    default:        te_printf(te, "[%02x]", c); break;
                    }
                    te_plt_use(te, Te_Plt_Scope_Status_Bar_Sup);
                }
            }
        } break;
    case Te_Mode_Insert:
        {
            char c = te_ws_buf_at(te, te_ws_ofs(te));
            te_printf( te,
                        " %2c (%d,%d)",
                        isprint(c) ? c : ' ',
                        te_get_cur_line(te, te_ws_ofs(te)),
                        te_get_cur_col(te, te_ws_ofs(te)) );
        } break;

    default:
        break;
    }
    te_plt_disable(te);

    /** 恢复光标 **/
    te_printf(te, RC);

    /** 显示光标 **/
    te_printf(te, CU_SHOW);

    return 0;
}


/**
 * @brief 运行命令模式
 * @param te 
 * @return int 
 */
static int _run_cmd_mode(te_t te)
{
    char c = te_getchar(te);
    te_char_push(te, &te->input, c);
    _try_exec_keys(te, te->keys[Te_Mode_Cmd], te_input_buf(te));
    return 0;
}

/**
 * @brief 运行在底行模式
 * @param te 
 * @return int 
 */
static int _run_last_line_mode(te_t te)
{
    char c = te_getchar(te);
    if(isprint(c))
        te_char_push(te, &te->cmd, c);
    else
        _try_exec_keys(te, te->keys[Te_Mode_Last_Line], &c);
    return 0;
}

/**
 * @brief 运行文本插入模式
 * @param te 
 * @return int 
 */
static int _run_insert_mode(te_t te)
{
    char c = te_getchar(te);
    te_char_push(te, &te->input, c);

    if(isprint(te_input_buf_at(te, 0)))
    {
        te_ins_str_on_disp(te, te_ws_ofs(te), &c, 1);   // 在屏幕上插入一个字符
        te_char_clear(te, &te->input);                  // 清空缓冲区
    }
    else
        _try_exec_keys(te, te->keys[Te_Mode_Insert], te_input_buf(te));

    _insert_mode_log(te);

    return 0;
}

/**
 * @brief 运行指定的工作模式
 * @param te 
 * @param mode 
 * @return int 
 */
static int _run_mode(te_t te)
{
    switch(te->mode)
    {
    case Te_Mode_Cmd:           _run_cmd_mode(te); break;
    case Te_Mode_Last_Line:     _run_last_line_mode(te); break;
    case Te_Mode_Insert:        _run_insert_mode(te); break;
    default: break;
    }
    return 0;
}















/**
 * @brief 初始化文本编辑器 
 * @param te 
 * @param init 
 * @return int 
 */
int te_init(te_t te, te_init_struct_t init)
{
    /** 结构体初始化 **/
    *te = (struct texteditor){0};
    te->ws.lbuf.buf     = init->workspace;
    te->ws.lbuf.len     = init->workspace_len;
    te->ws.lbuf.used    = 1;
    te->ws.lines        = 1;
    te->ws.lines_max    = 1;
    te->input.buf       = init->input_buf;
    te->input.len       = init->input_buf_len;
    te->cmd.buf         = init->cmd_buf;
    te->cmd.len         = init->cmd_buf_len;
    te->clip.buf        = init->clip_buf;
    te->clip.len        = init->clip_buf_len;
    for(int i = 0; i < _Te_Mode_Reverse; i++)
    {
        te->keys[i] = init->keys[i];
        te->cmds[i] = init->cmds[i];
    }
    te->ops  = init->ops;
    te_set_mode(te, Te_Mode_Cmd);     // 默认运行在命令模式

    /** 初始化调色板 **/
    if(!init->plts)
        init->plts = te_plt_for_sys();
    for(te_plt_scope_t s = 0; s < _Te_Plt_Scope_Reserve; s++)
    {
        if(init->plts[s])
            te_plt_get(te, s) = init->plts[s];
        else
            te_plt_get(te, s) = te_plt_for_sys()[s];
    }

    /** 启用备用屏幕 **/
    te_printf(te, USE_ALT_SCR);
    te_printf(te, CUP(1,1));

    /** 读取预定文件 **/
    te_file_load(te, init->filename, 0, te_ws_len(te));
    te_refr_ws_on_disp(te, 0, te_file_len(te), TE_RDF_CUR_TO_HOME);

    return 0;
}

/**
 * @brief 文本编辑器任务句柄
 * @note 该函数应当放在诸如 while(1) 的语句中被循环调用
 * @param te 
 * @return int 
 */
int te_task_handler(te_t te)
{
    _draw_line_nbr(te);             // 绘制行号
    _draw_status_bar_info(te);      // 绘制状态栏
    _run_mode(te);                  // 依据不同模式绘制工作区

    return 0;
}

/**
 * @brief 退出编辑器
 * @param te 
 * @return int 
 */
int te_quit(te_t te)
{
    te_printf(te, USE_NORMAL_SCR);      // 启用正常屏幕
    return te->ops->quit(te);
}

/**
 * @brief 设置编辑器模式
 * @param te 
 * @param mode 
 * @return int 
 */
int te_set_mode(te_t te, te_mode_t mode)
{
    switch(mode)
    {
    case Te_Mode_Cmd:         te_printf(te, CU_DIS_BLINK); break;
    case Te_Mode_Last_Line:   te_printf(te, CU_DIS_BLINK); break;
    case Te_Mode_Insert:      te_printf(te, CU_EN_BLINK); break;
    default: return -1;
    }
    te->mode = mode;
    return 0;
}

/**
 * @brief 将一定长度的字符串插入到工作区的指定偏移位置
 * @note 
 *      0. 该函数会重置编辑器偏移量的位置 \n
 *      1. 该函数将改变工作区的内容，但不会修改屏幕上的显示内容. \n
 *      2. 如果 ofs 超出工作区范围，则返回 -1. \n
 *      3. ws[ofs] 为要操作的起始位置，包括 ws[ofs] 位置在内后面的所有字符都将被移动，新的内容将会以 ws[ofs] 为起始开始填入. \n
 * @param te 
 * @param ofs 
 * @param str 
 * @param len 
 * @return te_ofs_t 返回插入新字符串后的偏移量
 */
te_ofs_t te_ins_str_at_ws(te_t te, te_ofs_t ofs, const char* str, int len)
{
    if(!te_is_ofs_valid(te, ofs))           // 检查偏移量的有效性
        return -1;
    if(te_is_end_of_ws(te, ofs + len))      // 检查操作范围的有效性，因为插入操作会扩大 used，因此此处的判定范围是整个工作区。
        return -1;

    for(int i = te_ws_used(te); i > ofs; i--)
        te_ws_buf_at(te, i + len - 1) = te_ws_buf_at(te, i - 1);
    memcpy(te_ws_buf(te) + ofs, str, len);

    int lines = 0;
    for(int i = 0; i < len; ++i)
        if(str[i] == '\n')
            lines++;

    te_ws_lines(te) += lines;
    if(te_ws_lines(te) > te_ws_lines_max(te))
        te_ws_lines_max(te) = te_ws_lines(te);
    te_ws_used(te) += len;
    te_ws_buf_at(te, te_ws_used(te)) = '\0';

    return (te_ws_ofs(te) = te_ws_ofs(te) + len);
}

/**
 * @brief 在工作区的指定偏移位置删除一定长度的字符串
 * @note 
 *      1. 该函数将改变工作区的内容，并更新工作区的偏移量，但不会修改屏幕上的显示内容. \n
 *      2. 如果 ofs 超出工作区范围，则返回 -1. \n
 *      3. ws[ofs-1] 为要操作的起始位置，包括 ws[ofs-1] 位置在内后面的所有字符都将被移动，后面内容将会以 ws[ofs-1] 为起始开始填入. \n
 *      
 * @param te 
 * @param ofs 
 * @param take 用于转存被移除的内容
 * @param len 
 * @return te_ofs_t 
 */
te_ofs_t te_del_str_at_ws(te_t te, te_ofs_t ofs, char* take, int len)
{
    if(!te_is_ofs_valid(te, ofs))           // 检查偏移量的有效性
        return -1;
    if(!te_is_ofs_valid(te, ofs + len))     // 检查操作范围的有效性
        return -1;

    if(take)
        memcpy(take, te_ws_buf(te) + ofs, len);

    int lines = 0;
    for(int i = 0; i < len; i++)
        if(te_ws_buf_at(te, i + ofs) == '\n')
            lines++;

    for(int i = ofs; i < te_ws_used(te); i++)
        te_ws_buf_at(te, i) = te_ws_buf_at(te, i + len);

    te_ws_lines(te) -= lines;
    te_ws_used(te) -= len;
    te_ws_buf_at(te, te_ws_used(te)) = '\0';

    return (te_ws_ofs(te) = ofs);
}

/**
 * @brief 在显示器上插入一个字符串
 * @note 该函数会向工作区插入字符串、重置编辑器偏移量、同步光标的位置
 * @param te 
 * @param c 
 * @return int 
 */
te_ofs_t te_ins_str_on_disp(te_t te, te_ofs_t ofs, const char* str, int len)
{
    if(!te_is_ofs_valid(te, ofs))           // 检查偏移量的有效性
        return -1;
    if(te_is_end_of_ws(te, ofs + len))      // 检查操作范围的有效性，因为插入操作会扩大 used，因此此处的判定范围是整个工作区。
        return -1;

    /** 向工作区插入一个字符串 **/
    te_ofs_t fin_ofs;       // 插入字符串完成后的偏移量
    if((fin_ofs = te_ins_str_at_ws(te, ofs, str, len)) < 0)
        return -1;

    /** 检查是否存在换行符 **/
    char* pos = strchr(str, '\n');

    if(!pos)
    {
        /** 若无换行符，则可以简化屏幕上字符串的显示，即插入空格后直接打印字符 **/
        te_printf(te, ICH(len));
        te_output(te, str, len);        // 由于 te_ins_str_at_ws() 已经完成了对偏移量更新，因此此处仅打印字符到屏幕即可
    }
    else
    {
        /** 若存在多个换行符，则需要化巧处理，避免全屏刷新 **/
        te_printf(te, CLL_BELOW);       // 清空当前行在光标之后的所有内容

        /** 打印插入的文本内容 **/
        for(int i = 0; i < len; i++)
        {
            te_putchar(te, str[i]);     // 由于 te_ins_str_at_ws() 已经完成了对偏移量更新，因此此处仅打印字符到屏幕即可

            switch(str[i])
            {
            case '\n':
                te_printf(te, IL(1));           // 插入新的一行，屏幕上光标以下原有显示内容自动下移
                te_cur_move_to_line_head(te);   // 重置光标列数，因为 te_putchar('\n') 换行后，光标将位于新一行的第一列
                break;
            }
        }

        /** 打印原来剩余的内容 **/
        int remain_ofs = fin_ofs;                           // 移动 ofs 到原来行后部剩余内容的首字符处
        te_ofs_t end = te_get_line_end(te, remain_ofs);     // 插入新字符后，行尾发生了变化，因此需要获取新的行尾
        for(te_ofs_t i = remain_ofs; i < end; i++)          // 打印到行尾的剩余内容
            te_putchar(te, te_ws_buf_at(te, i));            // 由于 te_ins_str_at_ws() 已经完成了对偏移量更新，因此此处仅打印字符到屏幕即可

        /** 重置光标位置到新插入字符串的末尾 **/
        te_reset_cur(te, fin_ofs);
    }

    return te_ws_ofs(te);
}

/**
 * @brief 在显示器上移除一个字符串
 * @note 该函数会向工作区移除字符串、重置编辑器偏移量、同步光标的位置
 * @example 例如：
 *              ofs     01234 5
 *              str:    hello[\0]
 *          要删除 ofs = 5, len = 1 的区域，因为越界，所以拒绝删除
 *          要删除 ofs = 0, len = 2 的区域，在界内，所有可以删除
 * @param te 
 * @param ofs 
 * @param buf 
 * @param len 
 * @return te_ofs_t 
 */
te_ofs_t te_del_str_on_disp(te_t te, te_ofs_t ofs, char* take, int len)
{
    /** 
     *  1. 统计途径的换行符的数量，由此可知要删除的行数，为 [\n]数量 + 1
     *  2. 获取 ofs 所在文本的行首 head，以及移除的末端处 stop
     *  3. 移除行，重置光标位置，打印重新打印从 head 到 stop 的内容
     *  4. 将移除的内容保存至 take
     *  5. 将工作区的内容删除
     */
    if(!te_is_ofs_valid(te, ofs))           // 检查偏移量的有效性
        return -1;
    if(!te_is_ofs_valid(te, ofs + len))      // 检查操作范围的有效性
        return -1;

    /** 擦除底部状态栏 **/
    _erase_status_bar_info(te);

    /** 获取要清空的行数 **/
    int lines = te_get_char_count(te, ofs, ofs + len, '\n');    // 将从要删除的内容中存在的换行符的数量
    te_ofs_t start = te_get_line_start(te, ofs);                // 获取行首

    /** 将光标移动至要清空的行首 **/
    te_reset_cur(te, start);

    /** 擦除/删除行内容 **/
    if(lines)
        te_printf(te, DL(lines));   // 有多少换行符，则需要删除多少行
    else
        te_printf(te, CLL_BELOW);   // 若换行符不存在，则仅清空光标后面的内容即可
    te_cur_move_to_line_head(te);   // 重置光标列数

    /** 移除工作区中的数据 **/
    te_ofs_t fin_ofs = te_del_str_at_ws(te, ofs, take, len);

    /** 重新打印剩余的内容 **/
    te_ofs_t end = te_get_line_end(te, fin_ofs);
    for(te_ofs_t i = start; i <= end; i++)  // 将移除内容后的新行的内容全部重新打印一次
    {
        char c = te_ws_buf_at(te, i);
        te_putchar(te, c);                  // 由于要保持光标位置，因此此处仅打印即可

        switch(c)
        {
        case '\n':
            te_cur_move_to_line_head(te);   // 重置光标列数
            break;
        }
    }

    /** 重置光标到移除数据后的位置 **/
    te_reset_cur(te, fin_ofs);

    return te_ws_ofs(te);
}

/**
 * @brief 重新刷新工作区的指定范围的内容到显示器上
 * @note 该函数刷新时会更新偏移量，但不对工作区的内容进行修改
 * @see 关于 @param flag，请参考 @file te_def.h 中的宏定义 @def TE_RDF_... 
 * @param te 
 * @param ofs 
 * @param len 
 * @param
 * @return int 
 */
int te_refr_ws_on_disp(te_t te, te_ofs_t ofs, int len, int flag)
{
    te_ofs_t ofs_origin = te_ws_ofs(te);
    te_reset_cur(te, ofs);

    int stop = ((len + ofs) > te_ws_used(te)) ? te_ws_used(te) : (len + ofs);
    for(int i = ofs; i < stop; i++)
    {
        char c = te_ws_buf_at(te, i);
        te_ws_putchar(te, c);

        switch(c)
        {
        case '\n':
            te_cur_move_to_line_head(te);   // 重置光标列数
            break;
        }
    }

    switch(flag)
    {
    case TE_RDF_CUR_TO_HOME:        te_reset_cur(te, 0); break;
    case TE_RDF_CUR_TO_INSERT_TAIL: te_reset_cur(te, ofs + len); break;
    case TE_RDF_CUR_TO_ORIGIN:      te_reset_cur(te, ofs_origin); break;
    }

    return 0;
}































