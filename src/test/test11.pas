program score(input, output);
var s:integer; ch:char;
begin
   s := 90;
   case s div 10 of
      10,9:ch := 'A';
      8:ch := 'B';
      7,6:ch := 'C';
      0,1,2,3,4,5:ch := 'D'
   end;
   write(s);
   write('-');
   write(ch)
end.