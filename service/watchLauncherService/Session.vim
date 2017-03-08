let SessionLoad = 1
if &cp | set nocp | endif
let s:cpo_save=&cpo
set cpo&vim
inoremap <expr> <Down> pumvisible() ? "\" : "\<Down>"
inoremap <expr> <C-S-Tab> pumvisible() ? "\" : "\<C-S-Tab>"
inoremap <expr> <Up> pumvisible() ? "\" : "\<Up>"
inoremap <S-Space> 
inoremap <expr> <C-Tab> pumvisible() ? "\" : "\<C-Tab>"
imap <silent> <expr> <F5> (pumvisible() ? "\<Plug>LookupFileCE" : "")."\\<Plug>LookupFile"
inoremap <Plug>LookupFileCE 
cmap <F2> set tags=
snoremap <silent>  c
xnoremap 	 :call UltiSnips#SaveLastVisualSelection()gvs
snoremap <silent> 	 :call UltiSnips#ExpandSnippet()
nmap  :BottomExplorerWindow
nmap  :FirstExplorerWindow
nmap d :cs find d =expand("<cword>")
nmap i :cs find i ^=expand("<cfile>")$
nmap f :cs find f =expand("<cfile>")
nmap e :cs find e =expand("<cword>")
nmap t :cs find t =expand("<cword>")
nmap c :cs find c =expand("<cword>")
nmap g :cs find g =expand("<cword>")
nmap s :cs find s =expand("<cword>")
map  <F6> :grep! =expand("<cword>")  ./*[chjq]*  -r  :cw 
map  <F7> :grep! =expand("<cword>") "%"   :cw 
nmap <silent>  	 :bprevious
map <silent> ;<F5> :w  :make  :cw  :cc 
nnoremap ;d :YcmShowDetailedDiagnostic
nmap ;ihn :IHN
nmap ;is :IHS:A
nmap ;ih :IHS
map <silent> ;ff :call FindFile(expand("<cword>"))
nnoremap ;gg :YcmCompleter GoToDefinitionElseDeclaration
nnoremap ;gf :YcmCompleter GoToDefinition
nnoremap ;gl :YcmCompleter GoToDeclaration
nmap <silent> ;fe :Explore
map <silent> ;qq :cclose
map <silent> ;p :cp 
map <silent> ;n :cn 
map <silent> ;  :cw
nmap ;<F2> :!pushfile.sh =expand("%:p") 
nmap <silent> ;m :A
map <silent> ;qa :qa 
map <silent> ;w :w 
map <silent> ;x :q 
nmap ;k :let g:LookupFile_TagExpr='"../filenametags"'
nmap <silent> ;lw :LUWalk
nmap <silent> ;ll :LUBufs
nmap <silent> ;lk :LUTags
nmap <silent> ;	 :bnext
nmap gx <Plug>NetrwBrowseX
nnoremap <silent> <Plug>NetrwBrowseX :call netrw#NetrwBrowseX(expand("<cWORD>"),0)
snoremap <silent> <Del> c
snoremap <silent> <BS> c
snoremap <silent> <C-Tab> :call UltiSnips#ListSnippets()
nmap <silent> <F5> <Plug>LookupFile
nmap <silent> <F9> :WMToggle
map <F12> :TlistAddFiles *.[ch]*
map <F8> ::TagbarToggle 
nmap <F3> :%s/\<=expand("<cword>")\>//g
map <F2> w
inoremap <silent> 	 =UltiSnips#ExpandSnippet()
imap ;ihn :IHN
imap ;is :IHS:A
imap ;ih :IHS
imap <silent> ;w  :w 
imap <silent> ;x  :x 
cmap ;s !cscope -bqk -i cscope.files 
cmap ;t !ctags -R `pwd` 
imap jj 
let &cpo=s:cpo_save
unlet s:cpo_save
set autoindent
set background=dark
set backspace=indent,eol,start
set completefunc=youcompleteme#Complete
set completeopt=menuone
set cpoptions=aAceFsB
set cscopeprg=/usr/bin/cscope
set cscopetag
set cscopetagorder=1
set cscopeverbose
set dictionary=/usr/share/dict/words
set expandtab
set fileencodings=utf-8,gb2312,gb18030,gbk,ucs-bom,cp936,latin1,unicode
set helplang=cn
set hlsearch
set ignorecase
set incsearch
set omnifunc=youcompleteme#OmniComplete
set path=.,/usr/include,,,~/work/service-developer/doc/service-developer-doc/demo/testService/**/*
set ruler
set runtimepath=~/.vim,~/.vim/bundle/Vundle.vim,~/.vim/bundle/YouCompleteMe,~/.vim/bundle/lookupfile,~/.vim/bundle/bufexplorer.zip,~/.vim/bundle/Tagbar,~/.vim/bundle/genutils,~/.vim/bundle/a.vim,~/.vim/bundle/winmanager,~/.vim/bundle/ultisnips,~/.vim/bundle/vim-snippets,/usr/share/vim/vimfiles,/usr/share/vim/vim74,/usr/share/vim/vimfiles/after,~/.vim/after,~/.vim/bundle/Vundle.vim,~/.vim/bundle/Vundle.vim/after,~/.vim/bundle/YouCompleteMe/after,~/.vim/bundle/lookupfile/after,~/.vim/bundle/bufexplorer.zip/after,~/.vim/bundle/Tagbar/after,~/.vim/bundle/genutils/after,~/.vim/bundle/a.vim/after,~/.vim/bundle/winmanager/after,~/.vim/bundle/ultisnips/after,~/.vim/bundle/vim-snippets/after
set shiftwidth=4
set showmatch
set smartcase
set tabstop=4
set tags=./tags,./TAGS,tags,TAGS,~/work/service-developer/doc/service-developer-doc/demo/testService/tags
set updatetime=2000
set viminfo=!,'100,<50,s10,h
let s:so_save = &so | let s:siso_save = &siso | set so=0 siso=0
let v:this_session=expand("<sfile>:p")
silent only
cd ~/work/service-developer/doc/service-developer-doc/demo/testService
if expand('%') == '' && !&modified && line('$') <= 1 && getline(1) == ''
  let s:wipebuf = bufnr('%')
endif
set shortmess=aoO
badd +0 ~/work/service-developer/doc/service-developer-doc/demo/testService/testService/src/DBusTest.cpp
silent! argdel *
edit ~/work/service-developer/doc/service-developer-doc/demo/testService/testService/src/DBusTest.cpp
set splitbelow splitright
set nosplitbelow
set nosplitright
wincmd t
set winheight=1 winwidth=1
argglobal
setlocal keymap=
setlocal noarabic
setlocal autoindent
setlocal nobinary
setlocal bufhidden=
setlocal buflisted
setlocal buftype=
setlocal cindent
setlocal cinkeys=0{,0},0),:,0#,!^F,o,O,e
setlocal cinoptions=
setlocal cinwords=if,else,while,do,for,switch
setlocal colorcolumn=
setlocal comments=sO:*\ -,mO:*\ \ ,exO:*/,s1:/*,mb:*,ex:*/,://
setlocal commentstring=/*%s*/
setlocal complete=.,w,b,u,t,i
setlocal concealcursor=
setlocal conceallevel=0
setlocal completefunc=youcompleteme#Complete
setlocal nocopyindent
setlocal cryptmethod=
setlocal nocursorbind
setlocal nocursorcolumn
setlocal nocursorline
setlocal define=
setlocal dictionary=
setlocal nodiff
setlocal equalprg=
setlocal errorformat=
setlocal expandtab
if &filetype != 'cpp'
setlocal filetype=cpp
endif
setlocal foldcolumn=0
setlocal foldenable
setlocal foldexpr=0
setlocal foldignore=#
setlocal foldlevel=0
setlocal foldmarker={{{,}}}
setlocal foldmethod=manual
setlocal foldminlines=1
setlocal foldnestmax=20
setlocal foldtext=foldtext()
setlocal formatexpr=
setlocal formatoptions=croql
setlocal formatlistpat=^\\s*\\d\\+[\\]:.)}\\t\ ]\\s*
setlocal grepprg=
setlocal iminsert=0
setlocal imsearch=0
setlocal include=
setlocal includeexpr=
setlocal indentexpr=
setlocal indentkeys=0{,0},:,0#,!^F,o,O,e
setlocal noinfercase
setlocal iskeyword=@,48-57,_,192-255
setlocal keywordprg=
setlocal nolinebreak
setlocal nolisp
setlocal nolist
setlocal makeprg=
setlocal matchpairs=(:),{:},[:]
setlocal modeline
setlocal modifiable
setlocal nrformats=octal,hex
setlocal nonumber
setlocal numberwidth=4
setlocal omnifunc=youcompleteme#OmniComplete
setlocal path=
setlocal nopreserveindent
setlocal nopreviewwindow
setlocal quoteescape=\\
setlocal noreadonly
setlocal norelativenumber
setlocal norightleft
setlocal rightleftcmd=search
setlocal noscrollbind
setlocal shiftwidth=4
setlocal noshortname
setlocal nosmartindent
setlocal softtabstop=0
setlocal nospell
setlocal spellcapcheck=[.?!]\\_[\\])'\"\	\ ]\\+
setlocal spellfile=
setlocal spelllang=en
setlocal statusline=
setlocal suffixesadd=
setlocal swapfile
setlocal synmaxcol=3000
if &syntax != 'cpp'
setlocal syntax=cpp
endif
setlocal tabstop=4
setlocal tags=
setlocal textwidth=0
setlocal thesaurus=
setlocal noundofile
setlocal nowinfixheight
setlocal nowinfixwidth
setlocal wrap
setlocal wrapmargin=0
silent! normal! zE
let s:l = 35 - ((34 * winheight(0) + 27) / 54)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
35
normal! 034|
tabnext 1
if exists('s:wipebuf')
  silent exe 'bwipe ' . s:wipebuf
endif
unlet! s:wipebuf
set winheight=1 winwidth=20 shortmess=filnxtToO
let s:sx = expand("<sfile>:p:r")."x.vim"
if file_readable(s:sx)
  exe "source " . fnameescape(s:sx)
endif
let &so = s:so_save | let &siso = s:siso_save
doautoall SessionLoadPost
unlet SessionLoad
" vim: set ft=vim :
