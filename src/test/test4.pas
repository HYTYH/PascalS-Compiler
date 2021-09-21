program Loop(input, output);
var a, number, sum: integer;
ub, lb, step: integer;
begin
   ub := 10;
   step := -2;
   number := ub;
   sum := 0;
   while number > lb do
   begin
      sum := sum + number;
      number := number + step
   end;
   writeln(sum);
   write(sum)
end.