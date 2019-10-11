program Lab2;

uses Math;

var q : integer;
var x, eps : real;

function get_atan(x, eps : real) : real;
var sign, tmp, n : integer;
var nth : real;
begin
    if (x < -1) or (x > 1) then Halt(-1);
    if (eps = 0) then Halt(-1);

    eps := Abs(eps);

    get_atan := 0;
    sign := 1;
    n := 0;

    while True do
    begin
        tmp := 2 * n + 1;

        nth := sign * (power(x, tmp) / tmp);
        if (Abs(nth) < eps) then Break;

        get_atan += nth;

        sign *= -1;
        Inc(n);
    end;
end;

begin
    q := 1;
    while q <> 0 do
    begin
        write('Input x: '); read(x);
        write('Input eps: '); read(eps);

        writeln('Answer: ', get_atan(x, eps));

        write('To quit enter 0: '); read(q);
    end;
end.
