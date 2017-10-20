if exists('g:loaded_soredake')
    finish
endif

" load soredake
let g:loaded_soredake = 1
" default value is enabled
let g:soredake_enable = get(g:, 'soredake_enable', 1)
" default value for maximum height
let g:soredake_maxheight = get(g:, 'soredake_maxheight', 6)

function! s:setup_window()
    botright split BufferList
    execute 'resize ' . min([len(g:buflist), g:soredake_maxheight])

    setlocal bufhidden=delete
    setlocal filetype=bufferlist
    setlocal buftype=nofile
    setlocal nomodifiable
    setlocal nocursorcolumn
    setlocal nonumber
    setlocal nobuflisted
    noswapfile
endfunction


function! s:display_bufferlist()
    setlocal modifiable

    let l:idx = 1
    for l:item in g:buflist
        if l:idx == g:curbufidx
            let l:current_sym = '>> '
        else
            let l:current_sym = '   '
        endif

        if l:item['changed']
            let l:changed_sym = '*'
        else
            let l:changed_sym = ' '
        endif

        silent! put! = l:current_sym . l:item['bufnr'] . '  ' . l:changed_sym
                    \. '  ' .l:item['name']

        normal! j
        let l:idx += 1
    endfor

    normal! ddgg
    execute ':' . g:curbufidx
    setlocal nomodifiable
endfunction

function! soredake#select_buffer()
    let l:idx = line('.') - 1
    let l:bufnr = g:buflist[l:idx]['bufnr']
    let l:winnr = bufwinnr(l:bufnr)

    call soredake#wipeout_goback()

    if l:winnr == -1
        execute 'buffer ' . l:bufnr
    else
        execute l:winnr . 'wincmd w'
    endif
endfunction

function! soredake#wipeout_goback()
    bwipeout
    execute g:lastwinnr . 'wincmd w'
endfunction

function! s:register_bufferlist_mappings()
    nnoremap <buffer> <Esc> :silent! call soredake#wipeout_goback()<Cr>
    nnoremap <buffer> <Space> :silent! call soredake#select_buffer()<Cr>
    nnoremap <buffer> <Cr> :silent! call soredake#select_buffer()<Cr>
endfunction

function! s:show_buffer_list(current)
    let g:buflist = getbufinfo({ 'buflisted': 1 })
    let g:lastwinnr = winnr()
    let g:curbufidx = 1

    let l:idx = 1
    for l:buf in g:buflist
        if a:current == l:buf['bufnr']
            let g:curbufidx = l:idx
            break
        endif

        let l:idx += 1
    endfor

    call s:setup_window()
    call s:display_bufferlist()
    call s:register_bufferlist_mappings()

    redraw
    let chr = getchar()
    let str = ''
    echom chr
    while chr != '\<Cr>'
        redraw
        let str = str . nr2char(chr)
        echo str
        let chr = getchar()
    endwhile
endfunction

function! soredake#enable()
    let g:soredake_enabled = 1
    command! BufferChangeList call s:show_buffer_list(bufnr('%'))
endfunction

if g:soredake_enable
    call soredake#enable()
endif

