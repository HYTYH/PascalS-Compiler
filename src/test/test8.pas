program varprocedure(input, output);
var x,y : integer;
procedure exchange(x,y:integer);
var tmp : integer;
begin
  tmp := x;
  x := y;
  y := tmp
end;
begin
  x := 1;
  y := 2;
  exchange(x,y);
  writeln(x);
  writeln(y)
end.