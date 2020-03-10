program Lab3;

uses StrUtils;

var q : integer;
var str : string;

function trunc_lex_sort(str : string) : string;
var b, e, c : integer;
var tmp : string;
begin
    str := ' ' + LowerCase(str) + ' ';
    trunc_lex_sort := '';
    b := 1;
    e := Length(str);
    c := 97;

    while Length(str) <> 1 do
    begin
        if c = 123 then Break;

        tmp := ' ' +  Chr(c);

        b := PosEx(tmp, str, b);

        if b = 0 then
        begin
            Inc(c);
            b := 1;
            Continue;
        end;

        e := PosEx(' ', str, b + 1);

        trunc_lex_sort += Copy(str, b, e - b);

        Delete(str, b, e - b);
    end;

    Delete(trunc_lex_sort, 1, 1);
end;

begin
    q := 1;
    while q <> 0 do
    begin
        Write('Input str: '); ReadLn(str);

        WriteLn('Answer: ', trunc_lex_sort(str));

        Write('To quit enter 0: '); ReadLn(q);
    end;
end.
