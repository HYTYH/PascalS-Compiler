program repeatUntilLoop(input,output);
var
   a: integer;
begin
   a := 10;
   repeat
      write('a');
      write('=');
      writeln(a);
      a := a + 1
   until a = 20;
end.