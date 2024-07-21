/**
 * @file te_palette.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-07-19
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "texteditor.h"

/**
 * @brief 设置编辑器的配色方案
 * @param te 
 * @param attr 
 * @param scope 
 * @return int 
 */
int te_plt_set(te_t te, const char* attr, te_plt_scope_t scope)
{
    if(te_plt_get(te, scope))
        te_plt_attr_get(te, scope) = attr;
    te_plt_use(te, scope);
    return 0;
}

/**
 * @brief 重置编辑器的所有配色方案
 * @param te 
 * @return int 
 */
int te_plt_reset(te_t te)
{
    for(te_plt_scope_t scope = Te_Plt_Scope_Status_Bar;
        scope < _Te_Plt_Scope_Reserve;
        ++scope)
    {
        te_plt_get(te, scope) = NULL;
    }
    te_plt_disable(te);
    return 0;
}

/**
 * @brief 使用编辑器的配色方案
 * @param te 
 * @param scope 
 * @return int 
 */
int te_plt_use(te_t te, te_plt_scope_t scope)
{
    if(te_plt_get(te, scope) && te_plt_attr_get(te, scope))
        te_printf(te, te_plt_attr_get(te, scope));
    else
        te_plt_disable(te);
    return 0;
}

/**
 * @brief 禁用编辑器的配色方案
 * @param te 
 * @return int 
 */
int te_plt_disable(te_t te)
{
    te_printf(te, TX_DEF);
    return 0;
}

/**
 * @brief 使用一个临时的配色方案
 * @note 这个函数不会影响到编辑器的配色方案，只会影响到下一次的输出
 * @param te 
 * @param attr 
 * @return int 
 */
int te_plt_use_temp(te_t te, const char* attr)
{
    te_printf(te, attr);
    return 0;
}
















