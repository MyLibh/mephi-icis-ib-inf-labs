program Lab4;

type int = integer;
type int_arr = array of int;

var N, i, q, c : int;
var seq : array of int;

function remove_zeroes(var seq : int_arr; N : int) : int_arr;
var new_n, i : int;
begin
    new_n := 0;
    for i := 0 to N do
    begin
        if seq[i] <> 0 then
        begin
            Inc(new_n);
            SetLength(remove_zeroes, new_n);
            remove_zeroes[new_n - 1] := seq[i];
        end;
    end;
end;

begin
  q := 1;
    while q <> 0 do
    begin
        Write('N: '); ReadLn(N);
        WriteLn('Input seq:');
        SetLength(seq, N);
        Dec(N);
        for i := 0 to N do
            Read(seq[i]);

        WriteLn('Answer: ');

        seq := remove_zeroes(seq, N);
        N := Length(seq) - 1;
        WriteLn('N: ', N + 1);
        for i := 0 to N do
            Write(seq[i], ' ');

        WriteLn();

        Write('To quit enter 0: '); ReadLn(q);
    end;
end.
