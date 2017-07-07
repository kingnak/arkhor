@echo off
pushd %~dp0

set SRC=%~dp0\bin

REM COMMON
REM ------------------------------------
set DEST=%~dp0\packages\org.kingnak.arkhor.common\data
rmdir /S /Q "%DEST%"
mkdir "%DEST%"
copy /Y "%SRC%\arkhorcommon.dll" "%DEST%"

REM SERVER
REM ------------------------------------
set DEST=%~dp0\packages\org.kingnak.arkhor.server\data
rmdir /S /Q "%DEST%"
mkdir "%DEST%"
copy /Y "%SRC%\arkhorserver.exe" "%DEST%"
copy /Y "%SRC%\arkhorscript.dll" "%DEST%"

REM SERVER SCRIPTS
REM ------------------------------------
set DEST=%~dp0\packages\org.kingnak.arkhor.server.data\data
rmdir /S /Q "%DEST%"
mkdir "%DEST%"
copy /Y "%SRC%\data.zip" "%DEST%"

REM SERVER TEST
REM ------------------------------------
set DEST=%~dp0\packages\org.kingnak.arkhor.server.test\data
rmdir /S /Q "%DEST%"
mkdir "%DEST%"
copy /Y "%SRC%\arkhorserver_test.exe" "%DEST%"

REM CLIENT
REM ------------------------------------
set DEST=%~dp0\packages\org.kingnak.arkhor.client\data
rmdir /S /Q "%DEST%"
mkdir "%DEST%"
copy /Y "%SRC%\arkhorclient.exe" "%DEST%"

REM CLIENT RES
REM ------------------------------------
set DEST=%~dp0\packages\org.kingnak.arkhor.client.res\data
rmdir /S /Q "%DEST%"
mkdir "%DEST%"
copy /Y "%SRC%\client_resources.rcc" "%DEST%"

REM CLIENT DATA
REM ------------------------------------
set DEST=%~dp0\packages\org.kingnak.arkhor.client.data\data
rmdir /S /Q "%DEST%"
mkdir "%DEST%"
copy /Y "%SRC%\res.zip" "%DEST%"

popd
