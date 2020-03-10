# Лабораторная работа №5.7

## Задание

Создать текстовый файл, в котором содержится некоторое количество строк. Переформатировать текст таким образом, чтобы каждая строка содержала не более 30 литер и заканчивалась точкой. Обеспечить вывод исходного текста и результата преобразования. Слова в файле должны сохранить первоначальный порядок следования и в них не должно быть знаков переноса.

## Блок-схема алгоритма
![flowchart](flowchart.png)

## Код

[Файл](lab.pas)

```pascal
program Lab5;

const
    INPUT_PATH  = 'input.txt';
    OUTPUT_PATH = 'output.txt';

var
    input, output : TextFile;
    line, cur, tmp : string;
    i : integer;

begin
    AssignFile(input, INPUT_PATH);
    AssignFile(output, OUTPUT_PATH);

    Reset(input);
    Rewrite(output);

    cur := '';

    while not EOF(input) do
    begin
        ReadLn(input, line);
        line := Trim(line);
        i := Pos(' ', line);
        while i > 0 do
        begin
            tmp := Trim(Copy(line, 1, i));
            if Length(cur) + Length(tmp) + 1 < 30 then
            begin
                if Length(cur) <> 0 then
                    cur += ' ';
                cur += tmp;
            end
            else
            begin
                if Length(cur) <> 0 then
                    WriteLn(output, cur, '.');
                cur := tmp;
            end;
            Delete(line, 1, i);
            line := Trim(line);
            i := Pos(' ', line);
        end;

        if Length(line) > 0 then
        begin
            if Length(cur) + Length(line) + 1 < 30 then
                begin
                    if Length(cur) <> 0 then
                        cur += ' ';
                    cur += line;
                end
          else
          begin
              if Length(cur) <> 0 then
                  WriteLn(output, cur, '.');
              cur := line;
          end;
        end;
    end;

    if Length(cur) <> 0 then
        WriteLn(output, cur, '.');

    CloseFile(input);
    CloseFile(output);
end.

```

## Тесты
