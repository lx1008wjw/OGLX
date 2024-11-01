@echo on
path %SCADEDISPLAY_HOME%\KCG67\bin;%PATH%
ScadeDisplayKCG.exe -outdir code -static AllObjects.sct AllObjects.sft AllObjects.sgfx AllObjects.sst AllObjects.stt AllObjects.swt AllObjects.sgt
