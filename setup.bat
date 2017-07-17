@echo off
echo Prepare Qt5
call setup\prepareqt5.bat
echo Prepare VS CRT
call setup\preparevs.bat
echo Prepare 3rd party
call setup\prepare3rdparty.bat
echo Build packages
call setup\buildpackages.bat
echo Build setup
call setup\buildsetup.bat

pause