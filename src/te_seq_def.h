/**
 * @file te_seq_def.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-07-16
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef __TE_SEQ_DEF_H__
#define __TE_SEQ_DEF_H__

/**
 * @brief 终端控制序列码
 */

/** 键码 **/
#define KEY_CTR_A       "\x01"
#define KEY_CTR_B       "\x02"
#define KEY_CTR_C       "\x03"
#define KEY_CTR_D       "\x04"
#define KEY_CTR_E       "\x05"
#define KEY_CTR_F       "\x06"
#define KEY_CTR_G       "\x07"
#define KEY_CTR_H       "\x08"
#define KEY_CTR_I       "\x09"
#define KEY_TAB         "\x09"
#define KEY_CTR_J       "\x0A"
#define KEY_CTR_K       "\x0B"
#define KEY_CTR_L       "\x0C"
#define KEY_CTR_M       "\x0D"
#define KEY_CTR_N       "\x0E"
#define KEY_CTR_O       "\x0F"
#define KEY_CTR_P       "\x10"
#define KEY_CTR_Q       "\x11"
#define KEY_CTR_R       "\x12"
#define KEY_CTR_S       "\x13"
#define KEY_CTR_T       "\x14"
#define KEY_CTR_U       "\x15"
#define KEY_CTR_V       "\x16"
#define KEY_CTR_W       "\x17"
#define KEY_CTR_X       "\x18"
#define KEY_CTR_Y       "\x19"
#define KEY_CTR_Z       "\x1A"
#define KEY_PAUSE       "\x1A"
#define KEY_ESC         "\033"
#define KEY_CSI         "\033["
#define KEY_BACKSPACE   "\x7F"
#define KEY_UP          KEY_CSI"A"
#define KEY_DW          KEY_CSI"B"
#define KEY_RIGHT       KEY_CSI"C"
#define KEY_LEFT        KEY_CSI"D"
#define KEY_HOME        KEY_CSI"H"
#define KEY_EMD         KEY_CSI"F"
#define KEY_CTR_UP      KEY_CSI"1;5A"
#define KEY_CTR_DW      KEY_CSI"1;5B"
#define KEY_CTR_RIGHT   KEY_CSI"1;5C"
#define KEY_CTR_LEFT    KEY_CSI"1;5D"
#define KEY_INSERT      KEY_CSI"2~"
#define KEY_DELETE      KEY_CSI"3~"
#define KEY_PAGE_UP     KEY_CSI"5~"
#define KEY_PAGE_DOWN   KEY_CSI"6~"
#define KEY_F1          "\033OP"
#define KEY_F2          "\033OQ"
#define KEY_F3          "\033OR"
#define KEY_F4          "\033OS"
#define KEY_F5          KEY_CSI"15~"
#define KEY_F6          KEY_CSI"17~"
#define KEY_F7          KEY_CSI"18~"
#define KEY_F8          KEY_CSI"19~"
#define KEY_F9          KEY_CSI"20~"
#define KEY_F10         KEY_CSI"21~"
#define KEY_F11         KEY_CSI"23~"
#define KEY_F12         KEY_CSI"24~"

/** 光标操作符，其中0x1B是ESC，只适用于xcmd_print函数 **/
#define CUU(n)      KEY_CSI"%dA",n		/** 光标向上	光标向上 <n> 行  **/
#define CUD(n)      KEY_CSI"%dB",n		/** 光标向下	光标向下 <n> 行  **/
#define CUF(n)      KEY_CSI"%dC",n		/** 光标向前	光标向前（右）<n> 行  **/
#define CUB(n)      KEY_CSI"%dD",n		/** 光标向后	光标向后（左）<n> 行  **/
#define CNL(n)      KEY_CSI"%dE",n		/** 光标下一行	光标从当前位置向下 <n> 行  **/
#define CPL(n)      KEY_CSI"%dF",n		/** 光标当前行	光标从当前位置向上 <n> 行  **/
#define CHA(n)      KEY_CSI"%dG",n		/** 绝对光标水平	光标在当前行中水平移动到第 <n> 个位置  **/
#define VPA(n)      KEY_CSI"%dd",n		/** 绝对垂直行位置	光标在当前列中垂直移动到第 <n> 个位置  **/
#define CUP(y,x)    KEY_CSI"%d;%dH",y,x	/** 光标位置	*光标移动到视区中的 <x>; <y> 坐标，其中 <x> 是 <y> 行的列  **/
#define HVP(y,x)    KEY_CSI"%d;%df",y,x	/** 水平垂直位置	*光标移动到视区中的 <x>; <y> 坐标，其中 <x> 是 <y> 行的列  **/
#define SC          KEY_CSI"s"          /** 保存光标位置  **/
#define RC          KEY_CSI"u"          /** 恢复光标位置  **/

/** 光标可见性 **/
#define CU_EN_BLINK     KEY_CSI"?12h"	/** ATT160	文本光标启用闪烁	开始光标闪烁  **/
#define CU_DIS_BLINK    KEY_CSI"?12l"	/** ATT160	文本光标禁用闪烁	停止闪烁光标  **/
#define CU_SHOW         KEY_CSI"?25h"	/** DECTCEM	文本光标启用模式显示	显示光标  **/
#define CU_HIDE         KEY_CSI"?25l"	/** DECTCEM	文本光标启用模式隐藏	隐藏光标  **/

/** 字符操作  **/
#define ICH(n)      KEY_CSI"%d@",n	/** 插入字符	在当前光标位置插入 <n> 个空格，这会将所有现有文本移到右侧。 向右溢出屏幕的文本会被删除。 **/
#define DCH(n)      KEY_CSI"%dP",n	/** 删除字符	删除当前光标位置的 <n> 个字符，这会从屏幕右边缘以空格字符移动。 **/
#define ECH(n)      KEY_CSI"%dX",n	/** 擦除字符	擦除当前光标位置的 <n> 个字符，方法是使用空格字符覆盖它们。 **/
#define IL(n)       KEY_CSI"%dL",n	/** 插入行	将 <n> 行插入光标位置的缓冲区。 光标所在的行及其下方的行将向下移动。 **/
#define DL(n)       KEY_CSI"%dM\r",n	/** 删除行	从缓冲区中删除 <n> 行，从光标所在的行开始。 **/
#define CLS         KEY_CSI"2J"     /** 清空整个屏幕  **/
#define CLS_BELOW   KEY_CSI"0J"     /** 清空从光标下方到屏幕末尾的内容  **/
#define CLS_ABOVE   KEY_CSI"1J"     /** 清空从屏幕起始到光标处的所有内容 **/
#define CLL         KEY_CSI"2K"     /** 清空整行  **/
#define CLL_BELOW   KEY_CSI"0K"     /** 清空从光标到行尾的内容  **/
#define CLL_ABOVE   KEY_CSI"1K"     /** 清空从行首到光标处的所有内容 **/

/** 打印字体颜色设置  **/
#define TX_DEF          KEY_CSI"0m"
#define TX_BLACK        KEY_CSI"30m"
#define TX_RED          KEY_CSI"31m"
#define TX_GREEN        KEY_CSI"32m"
#define TX_YELLOW       KEY_CSI"33m"
#define TX_BLUE         KEY_CSI"34m"
#define TX_WHITE        KEY_CSI"37m"

/** 字符设置  **/
#define TX_BOLD         KEY_CSI"1m"
#define TX_INVERSE      KEY_CSI"7m"

/** 打印背景颜色设置  **/
#define BK_DEF          KEY_CSI"0m"
#define BK_BLACK        KEY_CSI"40m"
#define BK_RED          KEY_CSI"41m"
#define BK_GREEN        KEY_CSI"42m"
#define BK_YELLOW       KEY_CSI"43m"
#define BK_BLUE         KEY_CSI"44m"
#define BK_WHITE        KEY_CSI"47m"

/** 备用屏幕 **/
#define USE_ALT_SCR     KEY_CSI"?1049h"     /** 启用备用屏幕 **/
#define USE_NORMAL_SCR  KEY_CSI"?1049l"     /** 启用正常屏幕 **/

#endif  // __TE_SEQ_DEF_H__
