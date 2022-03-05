define Token
    type    = Type
    i32_val = i32
    id_val  = string
end

"func" = Func
"is" = Is
"end" = End
"var" = Var

';' = Semicolon
'=' = Assign

<integer>     = IntL
<string>      = StringL
<non_keyword> = Id

