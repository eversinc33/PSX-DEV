    opt	m+,l.,c+

    section data            ; Store the array in the data section
    global char_sprite      ; Define label as global

char_sprite:
    incbin 'char.tim'       ; Include .TIM-file data 