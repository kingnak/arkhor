@echo off
pushd %~dp0

set SRC=%~dp0\..\3rdparty\compiled\release\msvc2017_64

REM COMMON
REM ------------------------------------
set DEST=%~dp0\packages\org.kingnak.arkhor.thirdparty.zip\data
rmdir /S /Q "%DEST%"
mkdir "%DEST%"
copy /Y "%SRC%\quazip.dll" "%DEST%"
copy /Y "%SRC%\zlib1.dll" "%DEST%"

popd
