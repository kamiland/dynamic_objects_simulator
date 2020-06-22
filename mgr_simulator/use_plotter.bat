@ECHO OFF
NSimulator.exe

IF NOT ERRORLEVEL 1 GOTO no_error
REM errorhandling, errorlevel >= 1
ECHO Complition Code: %ERRORLEVEL%.
GOTO end
:no_error
START /MIN python plotter.py
:end