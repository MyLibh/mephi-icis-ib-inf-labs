# Лабораторная работа №7.7

## Задание

На складе имеется список организаций, арендующих склад. Формат сведений об организации: организация, фамилия ответственного лица, количество метров, занимаемых организацией, дата поступления на склад продукции. Срок хранения на складе ограничен 30 днями. Разработать таблицу для поиска организаций, время хранения у которых закончилось. Ключ для каждой строки таблицы формируется из названия организации. Упорядочить строки таблицы по дате поступления продукции методом быстрой сортировки. Учтите, что каждая организация может завозить продукцию на склад любое число раз. Все пункты обработки сведений должны сопровождаться выводом информации.

## Блок-схема алгоритма
![flowchart](flowchart.png)

## Код

[Файл](lab.pas)

```pascal
program Lab7;

type
    Info = record
    surname : string;
    square : integer;
    date : integer;
    end;

    Node = record
        data : Info;
        next : ^Node := nil;
    end;

    type List = class
    private
        m_head : ^Node := nil;
        m_tail : ^Node := nil;
    public
        destructor delete;
        procedure add(val : Info);
        procedure print;
        function empty : boolean;
        procedure sort;
    end;

    Org = record
        company : string;
        list : List;
    end;

    Table = class
    private
    const
        N = 100;

        m_table : array[1..N] of Org;

        function get_hash(str : string) : word;
    public
        constructor Create();
        begin
            for var i := Low(m_table) to High(m_table) do
                m_table[i].list := List.Create();
        end;
        destructor delete;
        procedure add(str : string; val : Info);
        procedure print;
        procedure sort;
    end;

destructor List.delete;
begin
    m_tail := m_head;
    while m_tail <> nil do
    begin
        m_head := m_tail;
        m_tail := m_head^.next;
        Dispose(m_head);
    end;
end;

procedure List.add(val : Info);
var
    node : Node;
begin
    node.data := val;
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

procedure List.print;
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
            Write('(', ptr^.data.surname, '; ', ptr^.data.square, '; ', ptr^.data.date, ') ');

            ptr := ptr^.next;
        end;
    end;
end;

function List.empty() : boolean;
begin
  Result := (m_head = nil);
end;

procedure List.sort();
var
    node, ptr : ^Node;
    tmp : Info;
begin
    node := m_head;
    while node^.next <> nil do
    begin
        ptr := node;
        while ptr^.next <> nil do
        begin
            if ptr^.data.date < node^.data.date then
            begin
                tmp := ptr^.data;
                ptr^.data := node^.data;
                node^.data := tmp;
            end;

            ptr := ptr^.next;
        end;

        node := node^.next;
    end;
end;

function Table.get_hash(str : string) : word;
var
    sum : word := 0;
begin
    for var i := 1 to str.Length do
        sum += Ord(str[i]);

    Result := sum mod N;
end;

destructor Table.delete();
begin
    for var i := Low(m_table) to High(m_table) do
        m_table[i].list.delete();
end;

procedure Table.add(str : string; const val : Info);
var
    hash : word;
begin
    hash := get_hash(str);

    if m_table[hash].list.empty() then
        m_table[hash].company := str;

    m_table[hash].list.add(val);
end;

procedure Table.print();
begin
    WriteLn('Table');
    for var i := Low(m_table) to High(m_table) do
        if not m_table[i].list.empty() then
        begin
            Write(m_table[i].company, ' ');
            m_table[i].list.print();
            Write(' '#10);
        end;
end;

procedure Table.sort();
begin
    for var i := Low(m_table) to High(m_table) do
        if not m_table[i].list.empty() then
            m_table[i].list.sort();  
end;

const
    INPUT_PATH = 'input.txt';

var
    tab : Table;
    input : TextFile;
    tmp   : Info;
    company : string;
    tokens : array of string;

begin
    AssignFile(input, INPUT_PATH);
    Reset(input);
    tab := Table.Create;
    while not eof(input) do
    begin
        ReadLn(input, company);

        tokens := company.Split(' ');
        company := tokens[0];
        tmp.surname := tokens[1];
        tmp.square := StrToInt(tokens[2]);
        tmp.date := StrToInt(tokens[3]);

        tab.add(company, tmp);
    end;
    CloseFile(input);

    tab.print();
    tab.sort();
    tab.print();
    tab.delete();
end.
```

## Тесты
