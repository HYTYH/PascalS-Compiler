program sort (input,output);
var a : array[0..4] of integer;
x,y: integer;
procedure readarray;
var i : integer;
begin
  a[0] := 2;
  a[1] := 1;
  a[2] := 5;
  a[3] := 9;
  a[4] := 3
end;
procedure quicksort (l,h:integer);
var i,j,k,m: integer;
begin
    i := l;
    j := h;
    k := a[i];
    if l<h then 
    begin
        while i<j do
        begin
            while (a[j]>=k) and (i<j) do
            begin
                j := j-1
            end;
            a[i] := a[j];
            while (a[i]<=k) and (i<j) do
            begin
                i := i+1
            end;
            a[j] := a[i]
        end;
        a[i] := k;
        quicksort(l,i-1);
        quicksort(j+1,h)
    end
    else 
      m:=0
end;
begin 
    x := 0;
    readarray; 
    quicksort(0,4);
    while x<5 do
    begin
        y := a[x];
        x := x+1;
        writeln(y)
    end;
end.