program Lab1;

var x, k, q : integer;

(* function returning sum of 'k' lower digits in a number *)
function get_k_sum(x, k : integer) : integer;
begin
    if (k < 0) then Halt(-1);

    get_k_sum := 0;

    while  (k <> 0) do
    begin
        get_k_sum += (x mod 10);
        x := (x div 10);
        Dec(k);
    end;
end;

begin
    q := 1;
    while q <> 0 do
    begin
        write('Input x: '); read(x);
        write('Input k: '); read(k);

        writeln('Answer: ', get_k_sum(x, k));
        write('To quit enter 0: '); read(q);
    end;
end.
