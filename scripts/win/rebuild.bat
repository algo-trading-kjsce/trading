@echo off

set mydir=%~dp0%

call %mydir%\clean.bat

call %mydir%\build.bat %1