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

let s:str_list = []
let s:filtered_list = []
let s:search_string = ''
let s:this_path = expand('%:p:h:h')

function! s:setup_window()
    botright split BufferList
    execute 'resize ' . g:soredake_maxheight
    " execute 'resize ' . min([len(s:buflist), g:soredake_maxheight])

    setlocal bufhidden=delete
    setlocal filetype=bufferlist
    setlocal buftype=nofile
    setlocal nomodifiable
    setlocal nocursorcolumn
    setlocal nonumber
    setlocal nobuflisted
    noswapfile
endfunction


function! s:display_list(dictlist)
    setlocal modifiable
    normal! ggdG

    let pad = g:soredake_maxheight - len(a:dictlist)
    while pad > 0
        normal! o
        let pad -= 1
    endwhile 

    for item in a:dictlist
        silent! put! = item['str']
        normal! j
    endfor

    normal! ddz-
    setlocal nomodifiable
endfunction

function! s:create_bufferlist(buflist)
    let idx = 1
    let str = ''
    let lst = []

    for item in a:buflist
        if idx == s:curbufidx
            let str = '>> '
        else
            let str = '   '
        endif

        if item['changed']
            let changed_sym = '*'
        else
            let changed_sym = ' '
        endif

        let str = str . item['bufnr'] . '  ' . changed_sym . '  ' . item['name']
        call add(lst, { 'str': str, 'file': item['name'], 'int': [] })

        let idx += 1
    endfor

    return lst
endfunction

function! soredake#select_buffer()
    let l:idx = line('.') - 1

    if len(s:filtered_list) < line('$')
        let l:idx = l:idx - line('$') + len(s:filtered_list) 
    endif

    let l:bufnr = bufnr(s:filtered_list[l:idx]['file'])
    let l:winnr = bufwinnr(l:bufnr)

    call soredake#wipeout_buffer()

    if l:winnr == -1
        execute 'buffer ' . l:bufnr
    else
        execute l:winnr . 'wincmd w'
    endif
endfunction

function! s:register_bufferlist_mappings()
    nnoremap <buffer> <Esc> :call soredake#wipeout_buffer()<Cr>
    nnoremap <buffer> <Space> :call soredake#select_buffer()<Cr>
    nnoremap <buffer> <Cr> :call soredake#select_buffer()<Cr>
    nnoremap <buffer> <Tab> :call soredake#hen_search(s:str_list)<Cr>
endfunction

function! s:serialize(listdict)
    let lst = []
    for item in a:listdict
        call add(lst, item['file'])
        call add(lst, item['str'])
    endfor
    return lst
endfunction

function! s:call_hensearch(listdict, pattern)
    let strlist = s:serialize(a:listdict)
    let flt = system(s:this_path . '/searcher/searcher', [a:pattern] + strlist)

    let filtered = []

    if len(flt) == 0
        return a:listdict
    else
        let flt = split(flt, '\n')

        let filtered = []
        for lin in flt
            let i_split = split(lin, '-:-')
            let intervals_str = split(i_split[0], ' ')
            let intervals = []

            for intv in intervals_str
                call add(intervals, split(intv, ';'))
            endfor

            let item = { 'file': i_split[2], 'str': i_split[1], 'int': intervals }
            call add(filtered, item)
        endfor
    endif

    return filtered
endfunction

function! soredake#hen_search(list)
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
            call soredake#select_buffer()
            redraw!
            break
        elseif chr == g:soredake_ascii_mapping['tab']
            " Enter the buffer
            let s:search_string = str[3:]
            break
        else
            let s:filtered_list = s:call_hensearch(a:list, str[3:]) 
            call s:display_list(s:filtered_list)
        endif
    endwhile
endfunction

function! s:show_buffer_list(current)
    set laststatus=0

    let s:search_string = ''

    let s:bufdict = getbufinfo({ 'buflisted': 1 })
    let s:lastwinnr = winnr()
    let s:curbufidx = 1

    let l:idx = 1
    for l:buf in s:bufdict
        if a:current == l:buf['bufnr']
            let s:curbufidx = l:idx
            break
        endif

        let l:idx += 1
    endfor

    call s:setup_window()

    let s:str_list = s:create_bufferlist(s:bufdict)
    let s:filtered_list = s:call_hensearch(s:str_list, '')

    call s:display_list(s:filtered_list)
    call s:register_bufferlist_mappings()
    call soredake#hen_search(s:str_list)
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
