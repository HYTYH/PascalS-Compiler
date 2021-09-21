program example(input,output);
var 
    x : integer; u: array[0..4] of integer;
begin
    u[0] := 5;
    u[4] := 1000;
    x := u[0];
    u[0] := u[4];
    u[4] := x;
    write(u[0],u[4])
end.