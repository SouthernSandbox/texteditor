/**
 * @file te_plt_for_sys.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-07-19
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "texteditor.h"

struct te_palette _status_bar = 
{
    .attr = TX_DEF,
};

struct te_palette _status_bar_sup = 
{
    .attr = TX_YELLOW,
};

struct te_palette _workspace = 
{
    .attr = TX_DEF,
};

struct te_palette _line_nbr = 
{
    .attr = TX_DEF,
};

struct te_palette _line_nbr_curr = 
{
    .attr = TX_INVERSE,
};

struct te_palette _string_match = 
{
    .attr = TX_INVERSE,
};




te_plt_t te_plt_sys[] = 
{
    [Te_Plt_Scope_Status_Bar]       = &_status_bar,
    [Te_Plt_Scope_Status_Bar_Sup]   = &_status_bar_sup,
    [Te_Plt_Scope_Workspace]        = &_workspace,
    [Te_Plt_Scope_Line_Nbr]         = &_line_nbr,
    [Te_Plt_Scope_Line_Nbr_Curr]    = &_line_nbr_curr,
    [Te_Plt_Scope_String_Match]     = &_string_match,
};

te_plt_t* te_plt_for_sys(void)
{
    return te_plt_sys;
}
