#!/bin/sh

OUTFILE=badfont
rm -rf STAGING
rm *.ipk

mkdir STAGING

./buildit_for_emulator.sh

cp appinfo.json logo.png STAGING/
cp Build_Emulator/${OUTFILE} STAGING/
echo "filemode.755=${OUTFILE}" > STAGING/package.properties
palm-package STAGING

cp appinfo_portrait.json STAGING/appinfo.json
cp Build_Emulator/${OUTFILE} STAGING/
echo "filemode.755=${OUTFILE}" > STAGING/package.properties
palm-package STAGING
