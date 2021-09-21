program narcissus(input,output);
var i,a,b,c:integer;
begin
  for i := 100 to 999 do
  begin
    a := i div 100;
    b := i div 10 - a * 10;
    c := i mod 10;
    if i = a*a*a + b*b*b + c*c*c
    then writeln(i);
  end;
end.