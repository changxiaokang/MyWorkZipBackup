set background=dark
if version > 580
    hi clear
    if exists("syntax_on")
	syntax reset
    endif
endif
let g:colors_name="unique"

"highlight Normal guifg=grey87 guibg=#121420 gui=NONE ctermfg=253 ctermbg=233 cterm=NONE
"highlight Normal guifg=grey87 guibg=#121420 gui=NONE ctermfg=253 ctermbg=none cterm=NONE
highlight Normal guifg=grey85 guibg=#020410 gui=NONE ctermfg=253 ctermbg=none cterm=NONE

highlight ErrorMsg guifg=white guibg=red gui=NONE ctermfg=231 ctermbg=196 cterm=NONE
highlight VertSplit guifg=gray40 guibg=gray40 gui=NONE ctermfg=241 ctermbg=241 cterm=NONE
highlight FoldColumn guifg=tan guibg=grey30 gui=NONE ctermfg=180 ctermbg=239 cterm=NONE
highlight LineNr guifg=burlywood3 gui=NONE ctermfg=180 cterm=NONE
highlight MoreMsg guifg=seagreen gui=NONE ctermfg=29 cterm=NONE
highlight NonText guifg=cyan gui=NONE ctermfg=51 cterm=NONE
highlight Question guifg=springgreen gui=NONE ctermfg=48 cterm=NONE
highlight SpecialKey guifg=cyan gui=NONE ctermfg=51 cterm=NONE
highlight Title guifg=indianred gui=NONE ctermfg=167 cterm=NONE
highlight WarningMsg guifg=salmon gui=NONE ctermfg=210 cterm=NONE
highlight WildMenu guifg=gray guibg=gray17 gui=NONE ctermfg=250 ctermbg=235 cterm=NONE
highlight DiffAdd guifg=black guibg=wheat1 ctermfg=16 ctermbg=223
highlight DiffChange guifg=black guibg=skyblue1 ctermfg=16 ctermbg=117
highlight DiffText guifg=black guibg=hotpink1 gui=NONE ctermfg=16 ctermbg=205 cterm=NONE
highlight DiffDelete guifg=black guibg=gray45 gui=NONE ctermfg=16 ctermbg=243 cterm=NONE

" syntax highlighting groups
highlight Ignore guifg=grey40 gui=NONE ctermfg=241 cterm=NONE
highlight Todo guifg=orangered guibg=yellow2 gui=NONE ctermfg=202 ctermbg=226 cterm=NONE

"============================================================================
hi clear Error

"highlight Statement guifg=#decc72 gui=NONE ctermfg=185 cterm=NONE
"highlight Statement guifg=#decc72 gui=NONE ctermfg=184 cterm=NONE
"highlight Statement guifg=#d7d700 gui=NONE ctermfg=184 cterm=NONE
highlight Statement guifg=#c7c710 gui=NONE ctermfg=184 cterm=NONE
highlight Special guifg=#4eee72 gui=NONE ctermfg=84 cterm=NONE
highlight Comment guifg=limegreen gui=NONE ctermfg=40 cterm=NONE
highlight Cursor guifg=grey87 guibg=#00c800 gui=NONE ctermfg=253 ctermbg=40 cterm=NONE
highlight Identifier guifg=grey87 gui=NONE ctermfg=253 cterm=NONE
"highlight Function guifg=#7090ef gui=NONE ctermfg=69 cterm=NONE
highlight Function guifg=#5f87ff gui=NONE ctermfg=69 cterm=NONE
"highlight Constant guifg=#d591ef gui=NONE ctermfg=177 cterm=NONE
highlight Constant guifg=#d787ff gui=NONE ctermfg=177 cterm=NONE
highlight Type guifg=#0a9fff gui=NONE ctermfg=75 cterm=NONE
highlight Search guifg=black guibg=gold gui=NONE ctermfg=16 ctermbg=220 cterm=NONE
highlight IncSearch guifg=black guibg=gold gui=NONE ctermfg=16 ctermbg=220 cterm=NONE
highlight MatchParen guifg=yellow guibg=NONE gui=bold ctermfg=226 ctermbg=NONE cterm=bold
highlight Pmenu    guifg=white guibg=#445599 gui=NONE ctermfg=231 ctermbg=61 cterm=NONE
highlight PmenuSel guifg=limegreen guibg=#243579 ctermfg=40 ctermbg=18
highlight PreProc guifg=#ff5555 gui=NONE ctermfg=red cterm=NONE
highlight Folded guifg=grey65 guibg=grey17 gui=bold ctermfg=248 ctermbg=236 cterm=bold
highlight Conceal guifg=springgreen guibg=black gui=bold ctermfg=48 ctermbg=none cterm=NONE
highlight StorageClass guifg=tan1 ctermfg=215
highlight Visual guifg=white guibg=#0060df gui=NONE ctermfg=231 ctermbg=26 cterm=NONE
highlight CursorLine guifg=NONE guibg=grey25 gui=NONE ctermbg=238 cterm=NONE
highlight StatusLine guifg=white guibg=#082567 gui=bold ctermfg=231 ctermbg=17 cterm=bold
highlight StatusLineNC guifg=white guibg=gray30 gui=bold ctermfg=231 ctermbg=239 cterm=bold
highlight ModeMsg guifg=limegreen gui=NONE ctermfg=40 cterm=NONE
highlight Underlined guifg=NONE guibg=NONE gui=underline ctermfg=NONE cterm=underline

"内部函数
"hi _g_builtin   guifg=#7090ef   gui=italic
hi _g_builtin   guifg=#5f87ff   gui=italic
"运算符
"hi _g_operator  guifg=grey55               ctermfg=grey cterm=NONE
hi _g_operator  guifg=grey55                ctermfg=246 cterm=NONE
"内部异常
hi _g_exception guifg=grey87    gui=italic
"内部常量
hi _g_constant  guifg=#e5a1ff   gui=italic
"没用
hi _g_secstring guifg=#d591ef   gui=bold
"============================================================================
