program Hello(n);
var a: array[1..8] of integer;
begin
  a[2000] := 10;
  writeln(a[2000])
end.