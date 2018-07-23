@for /f %%i in (iamadog.txt) do @echo %%i && @MakePackageConsole.exe %%i
pause
del iamadog.txt
exit