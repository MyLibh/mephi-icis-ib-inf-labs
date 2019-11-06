# Лабораторная работа №6.7

## Задание

Задается некоторая последовательность чисел. Сохраняется последовательность в файле. Создать односвязный список. Найти в списке наибольший и наименьший элементы и поменять их местами. Результаты обработки списка и исходный список вывести на экран.

## Блок-схема алгоритма
![flowchart](flowchart.png)

## Код

[Файл](lab.pas)

```pascal
program Lab6;

type
    int = integer;
    Node = record
        val : int;
        next : ^Node;
    end;
    LinkedList = class
    private
        m_head : ^Node := nil;
        m_tail : ^Node := nil;
    public
        destructor delete;
        procedure add(val : int);
        procedure print;
        procedure swap_minmax;
    end;

destructor LinkedList.delete;
begin
    m_tail := m_head;
    while m_tail <> nil do
    begin
        m_head := m_tail;
        m_tail := m_head^.next;
        Dispose(m_head);
    end;
end;

procedure LinkedList.add(val : int);
var
    node : Node;
begin
    node.val := val;
    New(node.next);

    if m_head = nil then
    begin
        New(m_head);
        New(m_tail);
        m_head^ := node;
    end
    else
        m_tail^.next^ := node;

    m_tail^ := node;
end;

procedure LinkedList.print;
var
    ptr : ^Node;
begin
    ptr := m_head;
    if ptr = nil then
        Write('List is empty')
    else
    begin
        while ptr^.next <> nil do
        begin
            if ptr = m_tail then break;
            Write(ptr^.val, ' ');

            ptr := ptr^.next;
        end;
    end;
    Write(#10);
end;

procedure LinkedList.swap_minmax;
var
    prev, ptr, min, max, prevmin, prevmax : ^Node;
begin
    if m_head <> nil then
    begin
        min := m_head;
        max := m_head;
        ptr := m_head;
        prev := nil;
        while ptr^.next <> nil do
        begin
            if min^.val > ptr^.val then
            begin
                min := ptr;
                prevmin := prev;
            end;
            if max^.val < ptr^.val then
            begin
                max := ptr;
                prevmax := prev;
            end;
            prev := ptr;
            ptr := ptr^.next;
        end;

        ptr := prevmin^.next;
        prevmin^.next := max;
        max^.next := min^.next;

        prev := prevmax^.next;
        prevmax^.next := min;
        min^.next := max;
    end;
end;

const
    INPUT_PATH = 'input.txt';

var
    input : TextFile;
    list : LinkedList;
    val : int;

begin
    AssignFile(input, INPUT_PATH);
    Reset(input);
    list := LinkedList.Create;
    while not eof(input) do
    begin
        Read(input, val);
        list.add(val);
    end;
    CloseFile(input);

    list.print();
    list.swap_minmax();
    list.print();

    list.delete();
end.
```

## Тесты
