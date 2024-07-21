/**
 * @file te_config.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-07-16
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef __TE_CONFIG_H__
#define __TE_CONFIG_H__

/**
 * @brief 日志使能
 */
#define TE_LOG_ENABLED   1
#if (TE_LOG_ENABLED == 1)
    #include "stdio.h"
    #define TE_LOG(fmt, ...)   printf("[TE] " fmt "\r\n", ##__VA_ARGS__)
#endif

/**
 * @brief 行缓冲区大小
 */
#define TE_LINE_BUF_LEN    128

/**
 * @brief 命令缓冲区大小
 */
#define TE_CMD_BUF_LEN     32

/**
 * @brief 发送缓冲区大小
 */
#define TE_TX_BUF_LEN      128

/**
 * @brief 行号占用宽度
 */
#define TE_LINE_NUMBER_WIDTH    5

#endif  // __TE_CONFIG_H__
