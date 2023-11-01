#!/bin/bash

rm -rf STAGING
rm -f *.ipk
mkdir -p STAGING
rsync -r --exclude=.DS_Store --exclude=.svn ../enyo/ STAGING
./buildit_for_device.sh $1
cp Build_Device/hybridkeyboard_plugin STAGING
echo "filemode.755=hybridkeyboard_plugin" > STAGING/package.properties
palm-package STAGING
