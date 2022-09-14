@echo off
pushd %~dp0

REM TODO: Dynamic?
set SRC=C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\VC\Redist\MSVC\14.24.28127\x64\Microsoft.VC142.CRT

set DEST=%~dp0\packages\org.kingnak.arkhor.vs2019\data
rmdir /S /Q "%DEST%"
mkdir "%DEST%"
copy /Y "%SRC%\*" "%DEST%"

popd
