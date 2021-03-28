!;::
send ^a
sleep 100
send ^c
sleep 100

Haystack := clipboard
sleep 100

if(FoundPos := InStr(Haystack, "="))
{
	clipboard := "xxxx"
	sleep 100
	send {tab}
	sleep 100
	send ^v
	return
}

NeedleRegEx:="O)(([1-9]\d*\.?\d*)|(0\.\d*[1-9]))"
Match := {Len:{0: 0}}, FoundPos := 1, sum:=0

while(FoundPos := RegExMatch(Haystack, NeedleRegEx, Match, FoundPos + Match.Len[0]))
{
	sum := sum + Match.Value(0)
}

sleep 100
clipboard := sum
send {tab}
sleep 100
send ^v

return