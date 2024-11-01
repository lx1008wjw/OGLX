@echo off
path %SCADEDISPLAY_HOME%\KCG67\bin;%PATH%
ScadeDisplayKCG.exe -outdir code PrimaryFlightDisplay.sct PrimaryFlightDisplay.sft PrimaryFlightDisplay.sgfx PrimaryFlightDisplay.sst PrimaryFlightDisplay.stt PrimaryFlightDisplay.swt
