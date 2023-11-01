@echo off

rmdir /s /y STAGING
del *.ipk
mkdir STAGING

call buildit.cmd %1 %2

copy appinfo.json STAGING
copy %OUTFILE% STAGING
copy logo.png STAGING
echo filemode.755=%OUTFILE% > STAGING\package.properties
call palm-package STAGING

copy appinfo_portrait.json STAGING\appinfo.json
call palm-package STAGING
