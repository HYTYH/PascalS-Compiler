program inc(input, output);
var a,t:integer;
begin
   a := 1;
   t := 0;
   repeat
       t := t+1;
       a := (a * 21) mod 100;
   until t=2021;
write(a);
end.