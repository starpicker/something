@echo off

echo ���IE��ʱ�ļ�Ŀ¼...
del /f /s /q "%userprofile%\Local Settings\Temporary Internet Files\*.*"
del /f /s /q "%userprofile%\Local Settings\Temp\*.*"

echo �������ϵͳ��ʱ�ļ� *.tmp *._tmp *.log *.chk *.old �����Ե�...
del /f /s /q %systemdrive%\*.tmp
del /f /s /q %systemdrive%\*._mp
del /f /s /q %systemdrive%\*.log
del /f /s /q %systemdrive%\*.gid
del /f /s /q %systemdrive%\*.chk
del /f /s /q %systemdrive%\*.old

echo ��������䣬�����ļ���Ԥ����ű�...
del /f /s /q %systemdrive%\recycled\*.*
del /f /s /q %windir%\*.bak
del /f /s /q %windir%\prefetch\*.*
rd /s /q %windir%\temp & md %windir%\temp

rem cooke�������ʷ���Ǳ�����...
rem del /f /q %userprofile%\COOKIES s\*.*
rem del /f /q %userprofile%\recent\*.*



echo ����ϵͳ�������ļ�...
%windir%\system32\sfc.exe /purgecache

echo �Ż�Ԥ����Ϣ...
%windir%\system32\defrag.exe %systemdrive% -b

echo ���ϵͳ��ɣ�

echo. & pause
