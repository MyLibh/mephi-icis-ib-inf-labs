# Лабораторная работа №1.7

## Задание

Дано целое число __X__. Определить сумму __k__ младших цифр в записи числа.

## Блок-схема алгоритма
![flowchart](flowchart.png)

## Код

[Файл](lab.pas)

```pascal
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
```

## Тесты

Введем индикатор ошибки: -1.
* Заметим, что если __k__ < 0, то ошибка.
* Если __k__ > количества цифр __X__, то мы мысленно можем добавить лидирующие 0

### Testcase 1

|Step|   X|   k| ret|
|----|----|----|----|
|   0| 123|   2|   0|
|   1|  12|   1|   3|
|   2|   1|   0|   5|

### Testcase 2

|Step|   X|   k| ret|
|----|----|----|----|
|   0|4321|   4|   0|
|   1| 432|   3|   1|
|   2|  32|   2|   3|
|   3|   1|   1|   6|
|   4|   0|   0|  10|

### Testcase 3

|Step|   X|   k| ret|
|----|----|----|----|
|   0|   1|   1|   0|
|   1|   0|   0|   1|

### Testcase 4

|Step|   X|   k| ret|
|----|----|----|----|
|   0|  -7|   2|   0|
|   1|   0|   1|   7|
|   2|   0|   0|   7|

### Testcase 5

|Step|   X|   k| ret|
|----|----|----|----|
|   0|  12|  -1|   0|
|   0|  12|  -1|  -1|
