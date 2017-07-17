@echo off
pushd %~dp0

REM TODO: Dynamic?
set SRC=C:\Program Files (x86)\Microsoft Visual Studio\2017\Enterprise\VC\Redist\MSVC\14.10.25017\onecore\x64\Microsoft.VC150.CRT

set DEST=%~dp0\packages\org.kingnak.arkhor.vs2017\data
rmdir /S /Q "%DEST%"
mkdir "%DEST%"
copy /Y "%SRC%\msvcp140.dll" "%DEST%"
copy /Y "%SRC%\vccorlib140.dll" "%DEST%"
copy /Y "%SRC%\vcruntime140.dll" "%DEST%"

popd
