@echo off
pushd %~dp0

IF "%Qt5Dir64%" == "" (
	set QT5SRC=D:\Qt\5.15.2\msvc2019_64
) ELSE (
	set QT5SRC=%Qt5Dir64%
)

IF NOT EXIST "%QT5SRC%\bin\Qt5Core.dll" (
	echo Qt5Src is not set correctly
	GOTO :Exit
)

REM BASE
REM ------------------------------------
set QT5DEST=%~dp0\packages\org.kingnak.arkhor.qt5.base\data
rmdir /S /Q "%QT5DEST%"
mkdir "%QT5DEST%"
copy /Y "%QT5SRC%\bin\Qt5Core.dll" "%QT5DEST%"
copy /Y "%QT5SRC%\bin\Qt5Network.dll" "%QT5DEST%"

REM SERVER
REM ------------------------------------
set QT5DEST=%~dp0\packages\org.kingnak.arkhor.qt5.server\data
rmdir /S /Q "%QT5DEST%"
mkdir "%QT5DEST%"
copy /Y "%QT5SRC%\bin\Qt5Script.dll" "%QT5DEST%"

REM UI
REM ------------------------------------
set QT5DEST=%~dp0\packages\org.kingnak.arkhor.qt5.ui\data
rmdir /S /Q "%QT5DEST%"
mkdir "%QT5DEST%"
copy /Y "%QT5SRC%\bin\D3Dcompiler_47.dll" "%QT5DEST%"
copy /Y "%QT5SRC%\bin\libEGL.dll" "%QT5DEST%"
copy /Y "%QT5SRC%\bin\libGLESV2.dll" "%QT5DEST%"
copy /Y "%QT5SRC%\bin\opengl32sw.dll" "%QT5DEST%"
copy /Y "%QT5SRC%\bin\Qt5Gui.dll" "%QT5DEST%"
copy /Y "%QT5SRC%\bin\Qt5Widgets.dll" "%QT5DEST%"

mkdir "%QT5DEST%\platforms"
copy /Y "%QT5SRC%\plugins\platforms\qwindows.dll" "%QT5DEST%\platforms"

mkdir "%QT5DEST%\styles"
copy /Y "%QT5SRC%\plugins\styles\qwindowsvistastyle.dll" "%QT5DEST%\styles"

REM SERVER TEST
REM ------------------------------------
set QT5DEST=%~dp0\packages\org.kingnak.arkhor.qt5.server.ui\data
rmdir /S /Q "%QT5DEST%"
mkdir "%QT5DEST%"
copy /Y "%QT5SRC%\bin\Qt5ScriptTools.dll" "%QT5DEST%"

REM IMAGES
REM ------------------------------------
set QT5DEST=%~dp0\packages\org.kingnak.arkhor.qt5.ui.image\data
rmdir /S /Q "%QT5DEST%"
mkdir "%QT5DEST%"
mkdir "%QT5DEST%\imageformats"
copy /Y "%QT5SRC%\plugins\imageformats\qgif.dll" "%QT5DEST%\imageformats"
copy /Y "%QT5SRC%\plugins\imageformats\qjpeg.dll" "%QT5DEST%\imageformats"
copy /Y "%QT5SRC%\plugins\imageformats\qsvg.dll" "%QT5DEST%\imageformats"
copy /Y "%QT5SRC%\plugins\imageformats\qwbmp.dll" "%QT5DEST%\imageformats"
copy /Y "%QT5SRC%\plugins\imageformats\qwebp.dll" "%QT5DEST%\imageformats"

:Exit
popd

