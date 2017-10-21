if exists('g:loaded_soredake')
    finish
endif

" load soredake
let g:loaded_soredake = 1
" default value is enabled
let g:soredake_enable = get(g:, 'soredake_enable', 1)
" default value for maximum height
let g:soredake_maxheight = get(g:, 'soredake_maxheight', 6)

let g:soredake_ascii_mapping = {
            \'enter'    : 13,
            \'esc'      : 27,
            \'space'    : 32,
            \'tab'      : 9,
            \'backspace': "\<BS>",
            \'ignore'   : -42
            \}

let s:search_string = ''

function! s:setup_window()
    botright split BufferList
    execute 'resize ' . min([len(s:buflist), g:soredake_maxheight])

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
    for l:item in s:buflist
        if l:idx == s:curbufidx
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
    execute ':' . s:curbufidx
    setlocal nomodifiable
endfunction

function! soredake#select_buffer()
    let l:idx = line('.') - 1
    let l:bufnr = s:buflist[l:idx]['bufnr']
    let l:winnr = bufwinnr(l:bufnr)

    call soredake#wipeout_buffer()

    if l:winnr == -1
        execute 'buffer ' . l:bufnr
    else
        execute l:winnr . 'wincmd w'
    endif
endfunction

function! s:register_bufferlist_mappings()
    nnoremap <buffer> <Esc> :silent! call soredake#wipeout_buffer()<Cr>
    nnoremap <buffer> <Space> :silent! call soredake#select_buffer()<Cr>
    nnoremap <buffer> <Cr> :silent! call soredake#select_buffer()<Cr>
    nnoremap <buffer> <Tab> :call soredake#fuzzyfind()<Cr>
endfunction

function! soredake#fuzzyfind()
    " Redraw screen
    redraw

    " Grab input
    let str = '>> ' . s:search_string
    let chr = g:soredake_ascii_mapping['ignore']

    while v:true
        redraw

        if chr == g:soredake_ascii_mapping['backspace']
            " Delete last character only if we have typed something
            if len(str) > 3
                let str = str[:len(str) - 2]
            endif
        elseif chr != g:soredake_ascii_mapping['ignore']
            " Else, append it to the end of the string
            let str = str . nr2char(chr)
        endif
        echo str

        let chr = getchar()
        if chr == g:soredake_ascii_mapping['esc']
            " Cancel
            call soredake#wipeout_buffer()
            redraw!
            break
        elseif chr == g:soredake_ascii_mapping['enter']
            " Enter file, if exists
            call soredake#wipeout_buffer()
            redraw!
            break
        elseif chr == g:soredake_ascii_mapping['tab']
            " Enter the buffer
            let s:search_string = str[3:]
            break
        endif
    endwhile
endfunction

function! s:show_buffer_list(current)
    set laststatus=0
    let s:search_string = ''

    let s:buflist = getbufinfo({ 'buflisted': 1 })
    let s:lastwinnr = winnr()
    let s:curbufidx = 1

    let l:idx = 1
    for l:buf in s:buflist
        if a:current == l:buf['bufnr']
            let s:curbufidx = l:idx
            break
        endif

        let l:idx += 1
    endfor

    call s:setup_window()
    call s:display_bufferlist()
    call s:register_bufferlist_mappings()
    call soredake#fuzzyfind()
endfunction

function! soredake#wipeout_buffer()
    bwipeout
    set laststatus=2
    execute s:lastwinnr . 'wincmd w'
endfunction

function! soredake#enable()
    let g:soredake_enabled = 1
    command! BufferChangeList call s:show_buffer_list(bufnr('%'))
endfunction

if g:soredake_enable
    call soredake#enable()
endif
