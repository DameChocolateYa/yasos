syntax keyword yasosKeyword fun pub mkpub extern var cnst ret leave import use uhead llibrary libpath add adding sub subing mul muling div diving inc dec nwstruct if elif else while loop for stop continue eq neq bang gt gte lt lte undefa undefan and or ref deref new __asm def undef endif error warn isdef isndef sizeof struct goto
syntax region yasosString start=+"+ skip=+\\.+ end=+"+ keepend
syntax region yasosString start=+'+ skip=+\\.+ end=+'+ keepend
syntax match yasosNumber /\v(0x[0-9a-fA-F]+|\d+(\.\d+)?([eE][+-]?\d+)?)/
syntax match yasosFunction /\w\+\ze(/
syntax keyword yasosType int str double none nullptr true false list
syntax region yasosCommentLineFromSt start=+#-*.+ end=+$+ keepend
syntax match yasosCommentLineBefSt /^.*-#/
syntax match yasosCommentLine /^.*-#-.*$/
syntax region yasosCommentBlock start=+#\*+ end=+\*#+ keepend

highlight link yasosKeyword Keyword
highlight link yasosString String
highlight link yasosNumber Number
highlight link yasosFunction Function
highlight link yasosType Type
highlight link yasosCommentLineFromSt Comment
highlight link yasosCommentLineBefSt Comment
highlight link yasosCommentLine Comment
highlight link yasosCommentBlock Comment
