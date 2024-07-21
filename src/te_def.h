/**
 * @file te_def.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-07-16
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef __TE_DEF_H__
#define __TE_DEF_H__

#include "stdint.h"

#include "te_config.h"
#include "te_seq_def.h"

#define TE_UNUSED(x)    (void)x
#define TE_UNUSED_FUNC  __attribute__((unused))

struct texteditor;
typedef struct texteditor* te_t;
struct te_init_struct;
typedef struct te_init_struct* te_init_struct_t;
struct te_workspace;
typedef struct te_workspace* te_workspace_t;
struct te_line_buf;
typedef struct te_line_buf* te_line_buf_t;
struct te_key;
typedef struct te_key* te_key_t;
struct te_cmd;
typedef struct te_cmd* te_cmd_t;
struct te_ops;
typedef struct te_ops* te_ops_t;
struct te_file;
typedef struct te_file* te_file_t;
struct te_palette;
typedef struct te_palette* te_plt_t;

typedef int64_t te_fd_t;
typedef int te_ofs_t;
typedef int (*te_key_fn_t)(te_t te);
typedef int (*te_cmd_fn_t)(te_t te, int argc, char *argv[]);

/**
 * @brief 编辑器的工作模式
 */
typedef enum te_mode
{
    Te_Mode_Cmd = 0,        // 命令模式
    Te_Mode_Last_Line,      // 底行模式
    Te_Mode_Insert,         // 插入模式
    // Te_Mode_Scan,
    _Te_Mode_Reverse,
} te_mode_t;

/**
 * @brief 调色板作用域
 */
typedef enum te_plt_scope
{
    Te_Plt_Scope_Status_Bar,            // 状态栏
    Te_Plt_Scope_Status_Bar_Sup,        // 状态栏补充内容
    Te_Plt_Scope_Workspace,             // 工作区
    Te_Plt_Scope_Line_Nbr,              // 行号
    Te_Plt_Scope_Line_Nbr_Curr,         // 当前行号
    Te_Plt_Scope_String_Match,          // 字符串匹配
    _Te_Plt_Scope_Reserve,
} te_plt_scope_t;

/**
 * @brief 适用于 te_refr_ws_on_disp() 的特殊功能标记
 * @note RDF(Refresh Display Flag)
 */
#define TE_RDF_CUR_TO_HOME             0        // 刷新完成后，光标移动至 HOME 位置
#define TE_RDF_CUR_TO_INSERT_TAIL      1        // 刷新完成后，光标移动停留在刷新结束的位置
#define TE_RDF_CUR_TO_ORIGIN           2        // 刷新完成后，光标回到原位置

/**
 * @brief 适用于 struct texteditor::flag 的标记位
 */
#define TE_FLAG_NONE                    (0 << 0)
#define TE_FLAG_CATCH_KEY_HEAD          (1 << 0)    // 捕获到特殊组合键码的头部，例如，若获取到 "\xe0\x48" 且在某些组合键的头部含有 '\xe0' 时，该标记将被置位
#define TE_FLAG_LINE_NBR_SHOW           (1 << 1)    // 是否显示行数    


/**
 * @brief 行缓冲区
 */
struct te_line_buf
{
    char* buf;      // 缓冲区指针
    int len;        // 缓冲区长度
    int used;       // 缓冲区已使用的字节数
};

/**
 * @brief 工作区结构体
 */
struct te_workspace
{
    struct te_line_buf lbuf;    // 行缓冲区
    te_ofs_t ofs;               // 读写偏移量
    int lines;                  // 当前工作区的总行数
    int lines_max;              // 自运行以来的最大行数
};

/**
 * @brief 文件信息结构体
 */
struct te_file
{
    const char* name;       // 文件名
    te_fd_t fd;                // 文件描述符
    te_ofs_t ofs;           // 偏移量
    int len;                // 加载到工作区的长度
};

/**
 * @brief 调色板
 */
struct te_palette
{
    const char* attr;       // 配色属性
};

/**
 * @brief te 对象结构体
 */
struct texteditor
{
    te_key_t keys[_Te_Mode_Reverse];        // 组合键集
    te_cmd_t cmds[_Te_Mode_Reverse];        // 命令集
    struct te_file file;                    // 文件信息
    te_ops_t ops;                           // 操作函数

    struct te_workspace ws;         // 工作区
    struct te_line_buf input;       // 输入行缓冲区
    struct te_line_buf cmd;         // 命令缓冲区
    struct te_line_buf clip;        // 剪贴板缓冲区

    te_mode_t mode;         // 工作模式
    int flags;              // 标记位 

    te_plt_t plts[_Te_Plt_Scope_Reserve];   // 调色板
};

/**
 * @brief 用于初始化 te 对象的结构体
 */
struct te_init_struct 
{
    char* workspace;                        // 工作区缓冲区
    int workspace_len;                      // 工作区长度
    char* input_buf;                        // 行缓冲区
    int input_buf_len;                      // 行缓冲区长度
    char* cmd_buf;                          // 命令缓冲区
    int cmd_buf_len;                        // 命令缓冲区长度
    char* clip_buf;                         // 剪贴板缓冲区
    int clip_buf_len;                       // 剪贴板缓冲区长度
    te_key_t keys[_Te_Mode_Reverse];        // 组合键集
    te_cmd_t cmds[_Te_Mode_Reverse];        // 命令集
    const char* filename;                   // 预加载的文件的文件名，不需要时应为 NULL
    int read_ofs;                           // 指定文件的读写偏移位置
    int read_len;                           // 指定读取文件内容的长度 
    te_ops_t ops;                           // 操作函数集
    te_plt_t* plts;                         // 调色板
};

/**
 * @brief 功能组合键结构体
 * @note 适用于命令模式、插入模式
 */
struct te_key
{
    const char* key;        // 快捷键码
    int len;                // 键码的有效长度
    te_key_fn_t fn;         // 响应函数
};

/**
 * @brief 命令结构体
 * @note 适用于命令模式
 */
struct te_cmd
{
    const char* cmd;        // 命令名称
    int len;                // 命令名称的有效长度
    te_cmd_fn_t fn;         // 响应函数
};

/**
 * @brief te 操作函数集
 */
struct te_ops
{
    char (*getc)(void);         /** 获取一个字符 **/
    int (*putc)(char c);        /** 发送一个字符 **/

    te_fd_t (*open)(const char* name);
    int (*read)(te_fd_t fd, char* buf, int len);
    int (*write)(te_fd_t fd, const char* buf, int len);
    int (*lseek)(te_fd_t fd, int offset);
    int (*close)(te_fd_t fd);
    int (*remove)(const char* name);
    int (*rename)(const char* old_name, const char* new_name);
    int (*exist)(const char* name);

    int (*quit)(te_t te);
};

#endif  // __TE_DEF_H__
