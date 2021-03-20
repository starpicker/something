!'::
;send +{tab}
;send ^c

Haystack := clipboard

if (FoundPos := InStr(Haystack, "="))
{
	clipboard := "xxxx"
	send ^v
	return
}

NeedleRegEx:="O)(([1-9]\d*\.?\d*)|(0\.\d*[1-9]))"
Match := {Len: {0: 0}},  FoundPos := 1, sum := 0
While (FoundPos := RegExMatch(Haystack, NeedleRegEx, Match, FoundPos + Match.Len[0]))
{
   sum := sum + Match.Value(0)
}

clipboard := sum
;send ^v
   
return