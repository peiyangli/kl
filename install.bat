@echo off
set sysname=kmdf
net stop %sysname%
@echo %cd%
set sysfile=\??\%cd%\%sysname%64.sys
if exist "%Windir%\SysWOW64" goto start_install
set sysfile=\??\%cd%\%sysname%.sys

:start_install
@echo %sysfile%

Reg add "HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\%sysname%" /v ImagePath /t REG_SZ /d %sysfile%
Reg add "HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\%sysname%" /v ErrorControl /t REG_DWORD /d 0
Reg add "HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\%sysname%" /v DisplayName /t REG_SZ /d %sysname%
Reg add "HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\%sysname%" /v Start /t REG_DWORD /d 3
Reg add "HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\%sysname%" /v Type /t REG_DWORD /d 1

::Reg add "HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\%sysname%\Parameters\Wdf" /v WdfMajorVersion /t REG_DWORD /d 1
::Reg add "HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\%sysname%\Parameters\Wdf" /v WdfMinorVersion /t REG_DWORD /d 9
::Reg add "HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\%sysname%\Parameters\Wdf" /v TimeOfLastSqmLog /t REG_QWORD /d 0

net start %sysname%