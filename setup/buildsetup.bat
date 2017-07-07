@echo off
pushd %~dp0

IF "%QtIFW%" == "" (
	set IFW=D:\Qt\QtIFW2.0.5
) ELSE (
	set IFW=%QtIFW%
)

IF NOT EXIST "%IFW%\bin\binarycreator.exe" (
	echo QtIFW is not set correctly
	GOTO :Exit
)

IF EXIST "ArkHor Setup.exe" del /F /Q "ArkHor Setup.exe"
"%IFW%\bin\binarycreator.exe" -f -c config\config.xml -p packages "ArkHor Setup.exe"

:Exit
popd
