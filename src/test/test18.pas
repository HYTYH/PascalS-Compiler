program arraytest(n);
var b,c,i:integer; a: array[1..8] of integer;
begin
    for i := 0 to 7 do
    begin
        a[i] := i + 5
    end;
  writeln(a[1]);
  writeln(a[4])
end.