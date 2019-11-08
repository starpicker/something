@echo off 
for /f "delims=: tokens=2" %%i in (yuantuip.txt) do (
echo %%i:
ping %%i | find "¶ªÊ§)"
)
pause>nul