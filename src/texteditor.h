/**
 * @file texteditor.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-07-16
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef __TEXTEDITOR_H__
#define __TEXTEDITOR_H__

#include "te_def.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdbool.h"
#include "ctype.h"


#define TE_MAIN_VER     1
#define TE_SUB_VER      0
#define TE_UPDATE_STR   "2024.07.21"


#define te_ws_ofs(te)           (((te_t)te)->ws.ofs)
#define te_ws_buf(te)           (((te_t)te)->ws.lbuf.buf)
#define te_ws_buf_at(te, i)     (te_ws_buf(te)[i])
#define te_ws_len(te)           (((te_t)te)->ws.lbuf.len)
#define te_ws_used(te)          (((te_t)te)->ws.lbuf.used)
#define te_ws_lines(te)         (((te_t)te)->ws.lines)
#define te_ws_lines_max(te)     (((te_t)te)->ws.lines_max)

#define te_input_buf(te)        (((te_t)te)->input.buf)
#define te_input_buf_at(te, i)  (te_input_buf(te)[i])
#define te_input_len(te)        (((te_t)te)->input.len)
#define te_input_used(te)       (((te_t)te)->input.used)

#define te_cmd_buf(te)          (((te_t)te)->cmd.buf)
#define te_cmd_buf_at(te, i)    (te_cmd_buf(te)[i])
#define te_cmd_len(te)          (((te_t)te)->cmd.len)
#define te_cmd_used(te)         (((te_t)te)->cmd.used)

#define te_clip_buf(te)         (((te_t)te)->clip.buf)
#define te_clip_buf_at(te, i)   (te_clip_buf(te)[i])
#define te_clip_len(te)         (((te_t)te)->clip.len)
#define te_clip_used(te)        (((te_t)te)->clip.used)

#define te_file_name(te)        (((te_t)te)->file.name)
#define te_file_pos(te)         (((te_t)te)->file.ofs)
#define te_file_fd(te)          (((te_t)te)->file.fd)
#define te_file_len(te)         (((te_t)te)->file.len)

#define te_plt_get(te, i)       (((te_t)te)->plts[i])
#define te_plt_attr_get(te, i)  (((te_t)te)->plts[i]->attr)

#define te_set_flag(te, flag)   (((te_t)te)->flags |= (flag))
#define te_clr_flag(te, flag)   (((te_t)te)->flags &= ~(flag))
#define te_tst_flag(te, flag)   (((te_t)te)->flags & (flag))




int te_init(te_t te, te_init_struct_t init);
int te_task_handler(te_t te);
int te_quit(te_t te);
int te_set_mode(te_t te, te_mode_t mode);

te_ofs_t te_ins_str_at_ws(te_t te, te_ofs_t ofs, const char* str, int len);
te_ofs_t te_del_str_at_ws(te_t te, te_ofs_t ofs, char* take, int len);
te_ofs_t te_ins_str_on_disp(te_t te, te_ofs_t ofs, const char* str, int len);
te_ofs_t te_del_str_on_disp(te_t te, te_ofs_t ofs, char* take, int len);
int te_refr_ws_on_disp(te_t te, te_ofs_t ofs, int len, int flag);

te_key_t te_get_def_insmod_keys(void);
te_key_t te_get_def_cmdmod_keys(void);
te_key_t te_get_def_llmod_keys(void);

te_plt_t* te_plt_for_sys(void);

bool te_is_ofs_valid(te_t te, te_ofs_t ofs);
bool te_is_begin_of_ws(te_t te, te_ofs_t ofs);
bool te_is_end_of_ws(te_t te, te_ofs_t ofs);
int te_get_char_count(te_t te, te_ofs_t start, te_ofs_t stop, char c);
te_ofs_t te_find_char(te_t te, te_ofs_t start, te_ofs_t stop, char c);
te_ofs_t te_r_find_char(te_t te, te_ofs_t start, te_ofs_t stop, char c);
te_ofs_t te_ignore_char(te_t te, te_ofs_t start, te_ofs_t stop, char c);
te_ofs_t te_r_ignore_char(te_t te, te_ofs_t start, te_ofs_t stop, char c);
te_ofs_t te_get_line_start(te_t te, te_ofs_t ofs);
te_ofs_t te_get_line_end(te_t te, te_ofs_t ofs);
int te_get_line_len(te_t te, te_ofs_t ofs);
te_ofs_t te_get_prev_line_start(te_t te, te_ofs_t ofs);
te_ofs_t te_get_next_line_start(te_t te, te_ofs_t ofs);
te_ofs_t te_get_line_ofs(te_t te, int line);
int te_get_line_nbr_digits(te_t te, int nbr);
int te_get_cur_line(te_t te, te_ofs_t ofs);
int te_get_cur_col(te_t te, te_ofs_t ofs);
int te_get_cur_abs_line(te_t te, te_ofs_t ofs);
int te_get_cur_rel_line(te_t te, te_ofs_t ofs);
int te_get_cur_abs_col(te_t te, te_ofs_t ofs);
int te_get_cur_rel_col(te_t te, te_ofs_t ofs);

te_ofs_t te_prev_word(te_t te, te_ofs_t ofs);
te_ofs_t te_next_word(te_t te, te_ofs_t ofs);

te_ofs_t te_cur_up(te_t te, int n);
te_ofs_t te_cur_down(te_t te, int n);
te_ofs_t te_cur_left(te_t te, int n);
te_ofs_t te_cur_right(te_t te, int n);
te_ofs_t te_cur_move_to(te_t te, int line, int row);
te_ofs_t te_cur_back_home(te_t te);
int te_reset_cur(te_t te, te_ofs_t ofs);
te_ofs_t te_cur_move_to_line_head(te_t te);
te_ofs_t te_cur_move_to_line_tail(te_t te);


char te_getchar(te_t te);
int te_putchar(te_t te, const char c);
int te_ws_putchar(te_t te, const char c);
int te_puts(te_t te, const char *s);
int te_printf(te_t te, const char* fmt, ...);
int te_output(te_t te, const char* data, int len);
int te_ws_output(te_t te, const char* data, int len);
int te_char_push(te_t te, te_line_buf_t lbuf, const char c);
char te_char_pop(te_t te, te_line_buf_t lbuf);
int te_char_clear(te_t te, te_line_buf_t lbuf);

te_fd_t te_file_open(te_t te, const char *path);
int te_file_read(te_t te, te_fd_t fd, void* buf, int len);
int te_file_write(te_t te, te_fd_t fd, void* buf, int len);
int te_file_lseek(te_t te, te_fd_t fd, int ofs);
int te_file_close(te_t te, te_fd_t fd);
int te_file_remove(te_t te, const char* name);
int te_file_rename(te_t te, const char* old_name, const char* new_name);
int te_file_load(te_t te, const char* path, int pos, int len);
int te_file_sync(te_t te, const char* path);

int te_plt_set(te_t te,  const char* attr, te_plt_scope_t scope);
int te_plt_reset(te_t te);
int te_plt_use(te_t te, te_plt_scope_t scope);
int te_plt_use_temp(te_t te, const char* attr);
int te_plt_disable(te_t te);

#endif  // __TEXTEDITOR_H__
