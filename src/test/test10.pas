program llvm(input,output);
var a, i: integer;
procedure addX(x: integer); 
begin
    a := a + x;
end; 
begin
    a := 1;
    for i:= 1 to 200 do
    begin
        a := a + 1;
    end;
    while a <> 234 do
        a := a + 1;
    addX(20);
    writeln(a);
end.