"all system-wide defaults are set in $vimruntime/debian.vim (usually just
"/usr/share/vim/vimcurrent/debian.vim) and sourced by the call to :runtime
"you can find below.  if you wish to change any of those settings, you should
"do it in this file (/etc/vim/vimrc), since debian.vim will be overwritten
"everytime an upgrade of the vim packages is performed.  it is recommended to
"make changes after sourcing debian.vim since it alters the value of the
"'compatible' option.

" this line should not be removed as it ensures that various options are
" properly set to work with the vim-related packages available in debian.
runtime! debian.vim

" uncomment the next line to make vim more vi-compatible
" note: debian.vim sets 'nocompatible'.  setting 'compatible' changes numerous
" options, so any other options should be set after setting 'compatible'.
"set compatible

" vim5 and later versions support syntax highlighting. uncommenting the
" following enables syntax highlighting by default.
if has("syntax")
  syntax on
endif

" if using a dark background within the editing area and syntax highlighting
" turn on this option as well
"set background=dark

" uncomment the following to have vim jump to the last position when
" reopening a file
"if has("autocmd")
"  au bufreadpost * if line("'\"") > 1 && line("'\"") <= line("$") | exe "normal! g'\"" | endif
"endif

" uncomment the following to have vim load indentation rules and plugins
" according to the detected filetype.
"if has("autocmd")
 "filetype plugin indent on
"endif

" the following are commented out as they cause vim to behave a lot
" differently from regular vi. they are highly recommended though.
"set showcmd		" show (partial) command in status line.
"set showmatch		" show matching brackets.
set ignorecase		" do case insensitive matching
"set smartcase		" do smart case matching
set incsearch		" incremental search
"set autowrite		" automatically save before commands like :next and :make
"set hidden             " hide buffers when they are abandoned
"set mouse=a		" enable mouse usage (all modes)
autocmd FileType c,cpp,make,go,h set tabstop=4 | set expandtab
set shiftwidth=4
set hlsearch
set nonumber
"set autoindent
set smartcase
set nocp
"set foldenable
set foldmethod=manual
"set foldmethod=syntax
"set foldmethod=marker
"set foldcolumn=4
"set foldmethod=indent

set background=dark
"set background=light
"set filetype=c
let mapleader = ","
"map <silent> <leader>tl :TlistToggle<cr>

" source a global configuration file if available
if filereadable("/etc/vim/vimrc.local")
  source /etc/vim/vimrc.local
endif



""""""""""""""""""""""""""""""
" markbrowser setting
" """"""""""""""""""""""""""""""
 "nmap <silent> <leader>mk :MarksBrowser<cr>


""""""""""""""""""""""""""""""
" showmarks setting
""""""""""""""""""""""""""""""
" enable showmarks
" disable showmarks
"let showmarks_enable = 1
"" show which marks
"let showmarks_include = "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"
"" ignore help, quickfix, non-modifiable buffers
"let showmarks_ignore_type = "hqm"
"" hilight lower & upper marks
"let showmarks_hlline_lower = 1
"let showmarks_hlline_upper = 1


let tlist_auto_open=1
let tlist_auto_update=1
filetype plugin on
let &termencoding=&encoding
set fileencodings=utf-8,gbk,ucs-bom,cp936
"set tags=/home/arcsoft/Projects/LTE-Q/7th_workspace/DualCore/android/frameworks/base/tags
"set tags +=~/.vim/systags
"set tags =/home/lma/linux_else/tags
"hi Comment ctermfg =lightblue
"colorscheme desert
"colorscheme twilighted
colorscheme gruvbox

function! LoadCscope()
  let db = findfile("cscope.out", ".;")
  if (!empty(db))
    let path = strpart(db, 0, match(db, "/cscope.out$"))
    set nocscopeverbose " suppress 'duplicate connection' error
    exe "cs add " . db . " " . path
    set cscopeverbose
  endif
endfunction
au BufEnter /* call LoadCscope()

function! Zoom ()
    "check if is the zoomed state (tabnumber > 1 && window == 1)
    if tabpagenr('$') > 1 && tabpagewinnr(tabpagenr(), '$') == 1
        let l:cur_winview = winsaveview()
        let l:cur_bufname = bufname('')
        tabclose

        "restore the view
        if l:cur_bufname == bufname('')
            call winrestview(cur_winview)
        endif
    else
        tab split
    endif
endfunction

nmap <leader>z :call Zoom()<CR>

"ctrlp
let g:ctrlp_map = '<leader>p'
let g:ctrlp_cmd = 'CtrlP'
map <leader>r :CtrlPMRU<CR>

let g:ctrlp_custom_ignore = {
    \ 'dir':  '\v[\/]\.(git|hg|svn|rvm)$',
    \ 'file': '\v\.(exe|so|dll|zip|tar|tar.gz|pyc)$',
    \ }

let g:ctrlp_working_path_mode=0
let g:ctrlp_match_window_bottom=1
let g:ctrlp_max_height=15
let g:ctrlp_match_window_reversed=0
let g:ctrlp_mruf_max=500
let g:ctrlp_follow_symlinks=1

nnoremap <Leader>f :CtrlPFunky<Cr>
" narrow the list down with a word under cursor
"nnoremap <Leader>fU :execute 'CtrlPFunky ' . expand('<cword>')<Cr>
nnoremap <Leader>u :execute 'CtrlPFunky ' . expand('<cword>')<Cr>
let g:ctrlp_funky_syntax_highlight = 1
let g:ctrlp_extensions = ['funky']
let g:ctrlp_by_filename=1
let g:ctrlp_regexp=1

" Initialise list by a word under cursor
"nnoremap <Leader>u :execute 'CtrlPFunky ' . expand('<cword>')<Cr>
"noremap <silent><tab>m :tabnew<cr>
"noremap <silent><tab>e :tabclose<cr>
"noremap <silent><tab>n :tabn<cr>
"noremap <silent><tab>p :tabp<cr>
"noremap <silent><leader>t :tabnew<cr>
"noremap <silent><leader>g :tabclose<cr>
"noremap <silent><leader>1 :tabn 1<cr>
"noremap <silent><leader>2 :tabn 2<cr>
"noremap <silent><leader>3 :tabn 3<cr>
"noremap <silent><leader>4 :tabn 4<cr>
"noremap <silent><leader>5 :tabn 5<cr>
"noremap <silent><leader>6 :tabn 6<cr>
"noremap <silent><leader>7 :tabn 7<cr>
"noremap <silent><leader>8 :tabn 8<cr>
"noremap <silent><leader>9 :tabn 9<cr>
"noremap <silent><leader>0 :tabn 10<cr>
"noremap <silent><s-tab> :tabnext<CR>
"inoremap <silent><s-tab> <ESC>:tabnext<CR>

" Tab navigation
"nnoremap tp :tabprevious<CR>
"nnoremap tn :tabnext<CR>
"nnoremap to :tabnew<CR>
"nnoremap tc :tabclose<CR>
nnoremap t1 :tabn 1<cr>
nnoremap t2 :tabn 2<cr>
nnoremap t3 :tabn 3<cr>
"nnoremap gf <C-W>gf

" Move among windows
nnoremap <C-h> <C-W>h
nnoremap <C-j> <C-W>j
nnoremap <C-k> <C-W>k
nnoremap <C-l> <C-W>l
nnoremap <leader>. <C-W><C-W>

" NERDTree toggle
nnoremap <silent> <leader>n :NERDTreeToggle<CR>
nnoremap <silent> <leader>N :NERDTree .<CR>
nnoremap <silent> <leader>t :TlistToggle<CR>
nnoremap <silent> <leader>h :TlistToggle<CR><C-W>h

nnoremap <silent> <leader>q :q<CR>
nnoremap <silent> <leader>; :q<CR>
nnoremap <silent> <leader>w :wqa<CR>

"noremap <silent><leader>t :set listchars=tab:>-,trail:-<cr>
"nnoremap <silent><leader>t :set listchars=tab:>-,trail:$,extends:#,nbsp:%<cr> :set list<cr>
"nnoremap <silent><leader>T :retab!<cr>
"nnoremap <silent><leader>u :%s/\r\+$//e<cr>
"nnoremap <silent><leader>U :%s/\s*$//g<cr>


"nnoremap <leader>I :IndentLinesToggle<CR>
"nnoremap <leader>i :LeadingSpaceToggle<CR>
let g:indentLine_enabled = 0

" For vundle
set nocompatible
"set rtp+=~/.vim/bundle/vundle/
"call vundle#rc()
"Bundle 'gmarik/vundle'
" vim-scripts repos
"Bundle 'Valloric/YouCompleteMe'
"Bundle 'rdnetto/YCM-Generator'
"Bundle 'bling/vim-airline'
"Bundle 'dyng/ctrlsf.vim'

""
"let g:ycm_autoclose_preview_window_after_completion=1
""let g:ycm_global_ycm_extra_conf='~/.ycm_extra_conf.py'
"let g:ycm_collect_identifiers_from_tag_files=0
"let g:ycm_min_num_of_chars_for_completion=2
"let g:ycm_confirm_extra_conf=0

""let g:ycm_auto_trigger=0

"let g:ycm_enable_diagnostic_signs=0
"let g:ycm_enable_diagnostic_highlighting=0
"let g:ycm_echo_current_diagnostic=0
"let g:ycm_open_loclist_on_ycm_diags=0
"let g:ycm_autoclose_preview_window_after_insertion=0

"let g:ycm_show_diagnostics_ui=0

"let g:ycm_seed_identifiers_with_syntax=0
"let g:ycm_echo_current_diagnostic=0
"let g:syntastic_echo_current_error=1
""nnoremap <leader>gl :YcmCompleter GoToDeclaration<CR>
"nnoremap <leader>g :YcmCompleter GoToDefinition<CR>
"nnoremap <leader>d :YcmCompleter GoToDeclaration<CR>
"nnoremap <leader>F :YcmCompleter GoToInclude<CR>
"nnoremap <leader>D :YcmCompleter GoTo<CR>
""nnoremap <leader>gf :YcmCompleter GoToDefinition<CR>
""nnoremap <leader>gg :YcmCompleter GoToDefinitionElseDeclaration<CR>
"
"let g:ycm_filetype_blacklist = {
       "\ 'tagbar' : 1,
       "\ 'qf' : 1,
       "\ 'notes' : 1,
       "\ 'markdown' : 1,
       "\ 'unite' : 1,
       "\ 'text' : 1,
       "\ 'vimwiki' : 1,
       "\ 'pandoc' : 1,
       "\ 'infolog' : 1,
       "\ 'mail' : 1,
       "\ 'help' : 1,
       "\}
""\ 'make' : 1
""
let g:autocscope_menus=1

" easy motion
let g:EasyMotion_smartcase = 1
"let g:EasyMotion_startofline = 0 " keep cursor colum when JK motion
"map <leader><leader>h <Plug>(easymotion-linebackward)
"map <leader><leader>l <Plug>(easymotion-lineforward)
"map <leader><leader>s <Plug>(easymotion-s2)
"map <leader>. <Plug>(easymotion-s)
map s <Plug>(easymotion-s)
"map <leader><leader>t <Plug>(easymotion-t2)
"map <leader><leader>. <Plug>(easymotion-repeat)
"map  / <Plug>(easymotion-sn)
"omap / <Plug>(easymotion-tn)
"map  n <Plug>(easymotion-next)
"map  N <Plug>(easymotion-prev)

"a.vim
nnoremap <leader>a :A<CR>
nnoremap <leader>o :only<CR>

"ctrlsf.vim
"error happens
"let g:ctrlsf_default_root = 'project'
"nnoremap <leader>o :execute 'CtrlSF ' . expand('<cword>')<Cr>
"nnoremap <leader>t :CtrlSFToggle<CR>
nnoremap <Space> <C-f>

"map <F2> :call ToggleSketch()<CR>
"inoremap jj <ESC>

"nnoremap <Leader>e :call TracyoneGotoDef("")<cr>
"nnoremap <C-\>g :call TracyoneGotoDef("sp")<cr>

"function! TracyoneGotoDef(open_type)
    "let l:ycm_ret=s:YcmGotoDef(a:open_type)
    "if l:ycm_ret < 0
        "try
            "execute "cs find g ".expand("<cword>")
        "catch /^Vim\%((\a\+)\)\=:E/
            "call s:EchoWarning("cscope query failed")
            "if a:open_type != "" | wincmd q | endif
            "return -1
        "endtry
    "else
        "return 0
    "endif
    "return 0
"endfunction

"func! s:YcmGotoDef(open_type)
    "let l:cur_word=expand("<cword>")."\s*\(.*[^;]$"
    ":redir => l:msg
    "execute a:open_type
    "silent! execute ":YcmCompleter GoToDefinition"
    ":redir END
    "let l:rs=split(l:msg,'\r\n\|\n')
    "if get(l:rs,-1,3) !=3  && l:rs[-1] =~ 'Runtime.*'
        ":redir => l:msg
        "silent! execute ":YcmCompleter GoToDeclaration"
        ":redir END
        "let l:rs=split(l:msg,'\r\n\|\n')
        "if get(l:rs,-1,3) != 3 && l:rs[-1] !~ 'Runtime.*'
            "execute ":silent! A"
            "if search(l:cur_word) == 0
                "execute ":silent! A"
                "return -2
            "endif
            "return 3
        "elseif get(l:rs,-1,3) == 3  "not exist no error
            "return 0
        "else
            "return -3
        "endif

    "else
        "return 1
    "endif
"endfunc


autocmd FileType c,cpp map <buffer> <leader><space> :w<cr>:make<cr>
"set autochdir
let g:rainbow_active = 1
set shortmess=atI


set cursorline
"set cursorcolumn
hi CursorLine cterm=NONE ctermbg=brown ctermfg=white
set wildmenu


set updatetime=250
"let g:gitgutter_enabled = 0
"nnoremap <Leader>d :GitGutterToggle<Cr>
nnoremap <Leader>d :GitGutterNextHunk<Cr>
nnoremap <Leader>D :GitGutterPrevHunk<Cr>
nnoremap <Leader>U :GitGutterUndoHunk<Cr>
"has less use
"map <Leader>l :set invnumber<Cr>
"set splitright
"set splitbelow



"for airline
"let g:airline_theme="solarized"
let g:airline_theme="luna"
"let g:airline_theme="molokai"
let g:airline#extensions#tabline#enabled = 1
"nnoremap <C-p> :bn<CR>
nnoremap <leader>bb :BufExplorer<CR>
nnoremap <leader>bn :bn<CR>
nnoremap <leader>bp :bp<CR>
"nnoremap <C-i> :bp<CR>
"let g:airline#extensions#tabline#buffer_nr_show = 1
"let g:airline#extensions#tabline#left_sep = ' '
"let g:airline#extensions#tabline#left_alt_sep = '|'
"
"let g:airline#extensions#whitespace#enabled = 0
"let g:airline#extensions#whitespace#symbol = '!'
"
"
set hidden
let g:CtrlSpaceSearchTiming = 500
let g:CtrlSpaceLoadLastWorkspaceOnStart = 1
let g:CtrlSpaceSaveWorkspaceOnSwitch = 1
let g:CtrlSpaceSaveWorkspaceOnExit = 1
set showtabline=0
let g:airline_exclude_preview = 1

if executable("ag")
    let g:CtrlSpaceGlobCommand = 'ag -l --nocolor -g ""'
endif

" OmniCppComplete
set tags=./tags,tags;
"set autochdir
let OmniCpp_NamespaceSearch = 1
let OmniCpp_GlobalScopeSearch = 1
let OmniCpp_ShowAccess = 1
let OmniCpp_ShowPrototypeInAbbr = 1 " 显示函数参数列表
let OmniCpp_MayCompleteDot = 1   " 输入 .  后自动补全
let OmniCpp_MayCompleteArrow = 1 " 输入 -> 后自动补全
let OmniCpp_MayCompleteScope = 1 " 输入 :: 后自动补全
let OmniCpp_DefaultNamespaces = ["std", "_GLIBCXX_STD"]
" " 自动关闭补全窗口
"au CursorMovedI,InsertLeave * if pu"mvisible() == 0|silent! pclose|endif
set completeopt=menuone,menu,longest

set backspace=indent,eol,start

nnoremap <silent> <leader>cd :cd %:p:h<CR>

" Color name (:help cterm-colors) or ANSI code
let g:limelight_conceal_ctermfg = 'gray'
let g:limelight_conceal_ctermfg = 240

" Color name (:help gui-colors) or RGB color
let g:limelight_conceal_guifg = 'DarkGray'
let g:limelight_conceal_guifg = '#777777'

" Default: 0.5
let g:limelight_default_coefficient = 0.7

" Number of preceding/following paragraphs to include (default: 0)
let g:limelight_paragraph_span = 1

" Beginning/end of paragraph
"   When there's no empty line between the paragraphs
"   and each paragraph starts with indentation
let g:limelight_bop = '^\s'
let g:limelight_eop = '\ze\n\r^\s'

" Highlighting priority (default: 10)
"   Set it to -1 not to overrule hlsearch
let g:limelight_priority = -1

nmap <Leader>L <Plug>(Limelight)
xmap <Leader>L <Plug>(Limelight)

autocmd! User GoyoEnter Limelight
autocmd! User GoyoLeave Limelight!

"noremap <silent> <c-u> :call smooth_scroll#up(&scroll, 0, 2)<CR>
"noremap <silent> <c-d> :call smooth_scroll#down(&scroll, 0, 2)<CR>
"noremap <silent> <c-b> :call smooth_scroll#up(&scroll*2, 0, 4)<CR>
"noremap <silent> <c-f> :call smooth_scroll#down(&scroll*2, 0, 4)<CR>


"let g:sneak#label = 1
"map f <Plug>Sneak_s
"map F <Plug>Sneak_S
"map t <Plug>Sneak_t
"map T <Plug>Sneak_T
"
vmap <leader>y :w! ~/.vbuf<cr>
nmap <leader>y :.w! ~/.vbuf<cr>
nmap <leader>P :r ~/.vbuf<cr>

highlight diffAdded guifg=#00bf00
highlight diffRemoved guifg=#bf0000
set lazyredraw
"cabbrev gitv Gitv
"
"
"from git hub
" --user defined
"  ---------------------------------------------------------------

if filereadable(expand("~/.vimrc.local"))
  source ~/.vimrc.local
endif

" sudo then write
cabbrev w!! w !sudo tee % >/dev/null

" cd to the directory of the current buffer
"nnoremap <silent> <leader>cd :cd %:p:h<CR>
"
" switch between last two files
"dont touch tab for it is along with CTRL+I, will cause CANNOT jump back
"nnoremap <leader><Tab> <c-^>
"
nnoremap j gj
nnoremap k gk
"switch between windows by hitting <Tab> twice
"dont touch tab for it is along with CTRL+I, will cause CANNOT jump back
"nnoremap <silent> <Tab><Tab> <C-w>w
"nnoremap <silent> <Tab> <C-w>w
noremap Y y$
vnoremap < <gv
vnoremap > >gv
vmap <Tab> >
vmap <S-Tab> <
vmap q <ESC>
"noremap vv va{omxomz
"noremap ss [[?(<cr>b/<c-r><c-w><cr>N
"noremap qq f(b/<c-r><c-w><cr>N
"noremap tt <c-]>
"noremap tb <c-t>
"noremap ts :ts<cr>
"noremap tn :tn<cr>
"noremap tp :tp<cr>
"dont touch tab for it is along with CTRL+I, will cause CANNOT jump back
"noremap <Tab> :tn<cr>
"noremap <S-Tab> :tp<cr>
set shortmess +=F

nmap <leader>/ :lv /<c-r>=expand("<cword>")<cr>/ %<cr>:lw<cr>
nmap <leader>lv :exec 'lvimgrep /' . input('/', expand('<cword>')) . '/j % <bar> lopen'<cr>

function! TabMessage(cmd)
  redir => message
  silent execute a:cmd
  redir END
  if empty(message)
    echoerr "no output"
  else
    " use "new" instead of "tabnew" below if you prefer split windows instead of tabs
    tabnew
    setlocal buftype=nofile bufhidden=wipe noswapfile nobuflisted nomodified
    silent put=message
  endif
endfunction

command! -nargs=+ -complete=command TMessage call TabMessage(<q-args>)


function! WindowMessage(...)
" this function output the result of the Ex command into a split scratch buffer
  let cmd = join(a:000, ' ')
  let temp_reg = @"
  redir @"
  silent! execute cmd
  redir END
  let output = copy(@")
  let @" = temp_reg
  if empty(output)
    echoerr "no output"
  else
    new
    setlocal buftype=nofile bufhidden=wipe noswapfile nobuflisted
    put! =output
  endif
endfunction
command! -nargs=+ -complete=command WindowMessage call WindowMessage(<f-args>)

"let g:ag_prg="ag --vimgrep -s -w"
let g:ag_prg="ag --vimgrep -S"
let g:ag_working_path_mode='r'
let g:ag_highlight=1

let g:EclimCValidate = 0
let g:EclimFileTypeValidate = 0
let g:EclimBrowser = 'w3m'
let g:EclimQuickFixHeight = 15
let g:EclimProjectTreeAutoOpen = 0
let g:EclimProjectTreeActions = [
    \ {'pattern': '.*', 'name': 'Split', 'action': 'split'},
    \ {'pattern': '.*', 'name': 'Tab', 'action': 'tabnew'},
    \ {'pattern': '.*', 'name': 'Edit', 'action': 'edit'},
  \ ]

"you can instead Todo to what you want
let g:EclimTodoSearchPattern = '\(\<fixme\>\|\<todo\>\|\<Todo\>\)\c'

"let g:EclimTodoSearchExtensions = ['java', 'py', 'php', 'jsp', 'xml', 'html']
let g:EclimTodoSearchExtensions = ['java', 'c', 'cpp']

command -range -nargs=* Bd call eclim#web#SearchEngine(
	\ 'http://www.baidu.com/s?wd=<query>', <q-args>, <line1>, <line2>)

nmap <leader>m <c-o>
nmap <leader>i <c-i>

nmap <leader>cn :cn<cr>
nmap <leader>j :cn<cr>
nmap <leader>cp :cp<cr>
nmap <leader>k :cp<cr>
nmap <leader>ch :CCallHierarchy<cr>
"nmap <leader>cd :CSearchContext<cr>
nmap <leader>l :CSearchContext<cr>
nmap <leader>cs :CSearch <c-r><c-w> -x all<cr>
"nmap <leader>l :CSearch <c-r><c-w> -x all<cr>
nmap <leader>cf :LocateFile <c-r><c-w>.h<cr>
nmap <leader>T :ProjectsTree<cr>

"nmap <leader>C :bp<cr>:q<cr>
nmap <leader>C :-quit<cr>
nmap <leader>lu :Ag <c-r><c-w><cr>


nmap ++ <c-w>h<c-w>_<c-w>><<c-w>>c-w>>
nmap -- <c-w>l<c-w>_<c-w>><c-w>><c-w>>
set wildmode=list:longest,full
"set nowrapscan
"
"set UltiSnips
let g:UltiSnipsExpandTrigger="<leader><tab>"
let g:UltiSnipsJumpForwardTrigger="<leader><tab>"
let g:UltiSnipsJumpBackwardTrgger="<leader><tab>"
let g:UltiSnipsListSnippets="<c-e>"

nmap <leader>Y :YRShow<cr>
"let g:yankring_history_dir =$HOME.'/.yanking/'
let g:yankring_history_file = '.yankring_history'

nmap vv ]c
nmap VV [c
"nmap <space>j ]c
"nmap <space>k [c

nmap "" ebi"<esc>ea"<esc>

let g:color_coded_enabled = 1
let g:color_coded_filetypes = ['c', 'cpp', 'objc']

" Disable color_coded in diff mode
if &diff
  let g:color_coded_enabled = 0
endif

nmap <leader>H :TagbarToggle<CR>

"for syntastic
set statusline+=%#warningmsg#
set statusline+=%{SyntasticStatuslineFlag()}
set statusline+=%*

let b:syntastic_skip_checks = 0
let g:syntastic_always_populate_loc_list = 0
let g:syntastic_auto_loc_list = 0
let g:syntastic_check_on_open = 0
let g:syntastic_check_on_wq = 0
let g:syntastic_enable_signs = 1
let g:syntastic_enable_highlighting = 1
let g:syntastic_cpp_checkers = ['gcc']
let g:syntastic_cpp_compiler = 'clang'
let g:syntastic_cpp_compiler_options = ' -std=c++17 -stdlib=libc++'
let g:syntastic_mode_map = {
        \ "mode": "passive",
        \ "active_filetypes": [],
        \ "passive_filetypes": [] }



"for indent_guides
hi IndentGuidesOdd  ctermbg=black
hi IndentGuidesEven ctermbg=darkgrey

let g:startify_custom_header = []
vmap <leader>S :!astyle<CR>
