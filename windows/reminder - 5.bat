@echo off
setlocal enabledelayedexpansion

set work=check yang

set time=14:30

set msgbox=mshta vbscript:msgbox(\"you should do !work!\",600,\"reminder\")(window.close)

schtasks /create -f /tn "reminder" /tr "!msgbox!" /sc once /st !time!

pause

rem schtasks /delete /TN reminder
rem schtasks /delete /TN reminder