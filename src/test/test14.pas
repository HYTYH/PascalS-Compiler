program calendar(input,output);
var year,month,days: integer;
begin
  year := 2021;
  month := 6;
  case month of
    1,3,5,7,8,10,12: days := 31;
    4,6,9,11 : days :=30;
    2 : if (year mod 4 = 0) and (year mod 100 <> 0) or (year mod 400 = 0)
        then days := 29
        else days :=28
  end;
  write(days)
end.