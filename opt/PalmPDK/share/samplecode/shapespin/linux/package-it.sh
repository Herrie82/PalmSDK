#!/bin/bash

if [ ! -e plugin ];then
cd ..
fi

rm -rf STAGING
rm *.ipk
mkdir -p STAGING
rsync -r --exclude=.DS_Store --exclude=.svn enyo/ STAGING/
# rsync -r --exclude=.DS_Store --exclude=.svn mojo/ STAGING/
./linux/buildit_for_device.sh
cp shapespin_plugin STAGING
echo "filemode.755=shapespin_plugin" > STAGING/package.properties
palm-package STAGING
