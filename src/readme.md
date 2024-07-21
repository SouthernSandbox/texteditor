# 文件结构
```
./
 ├─ te_config.h                // 关于编辑器的一些配置
 ├─ te_def.h                   // 关于编辑器的一些结构体、枚举、标志位等的定义
 ├─ te_seq_def.h               // 关于终端控制序列的定义
 ├─ texteditor.h               // TextEditor 库的头文件，声明了一些访问成员的宏和功能函数
 │
 ├─ te_core.c                  // 运行逻辑核心
 │
 ├─ te_cur.c                   // 关于光标的控制
 ├─ te_file_ops.c              // 关于文件的操作
 ├─ te_ofs_ops.c               // 关于工作区偏移量的操作
 ├─ te_io.c                    // 关于输入输出的操作
 ├─ te_palette.c               // 关于终端彩色输出的操作
 │
 ├─ te_keys_for_cmd.c          // 适用于命令模式的组合键功能的代码实现
 ├─ te_keys_for_insert.c       // 适用于插入模式的组合键功能的代码实现
 ├─ te_keys_for_last_line.c    // 适用于底行模式的组合键功能的代码实现
 └─ te_plt_for_sys.c           // 编辑器默认的调色板
```
