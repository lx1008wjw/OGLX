@echo off
@echo.
@echo --------------------------------------------------------------

cd %~dp0..
set SIGNATURE_FILE=.\\bin\SDYOGLX67signature.md5
if not exist "%SIGNATURE_FILE%" goto noSignatureFile
".\\bin\md5deep_et.exe" -q "%SIGNATURE_FILE%"
set status=%ERRORLEVEL%
@echo -------------------------------------------------------------------------------------
@echo.

if "%1"=="-batch" goto exitCmd
goto pauseCmd

:noSignatureFile
@echo    ERROR: %SIGNATURE_FILE% does not exist

:pauseCmd
pause

:exitCmd
cd .\\bin
exit /B
