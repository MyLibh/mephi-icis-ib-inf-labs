program Lab1;

var x, k : integer;

(* function returning sum of 'k' lower digits in a number *)
function get_k_sum(x, k : integer) : integer;
begin
    if (k < 0) then exit(-1);

    get_k_sum := 0;

    while  (k <> 0) do
    begin
        get_k_sum += (x mod 10);
        x := (x div 10);
        Dec(k);
    end;
end;

begin
  write('Input x: '); read(x);
  write('Input k: '); read(k);

  writeln('Answer: ', get_k_sum(x, k));
end.
