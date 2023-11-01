STAGING_DIR=STAGING/com.palmdts.app.simplepdk

rm -rf $STAGING_DIR
rm *.ipk
mkdir -p $STAGING_DIR
./buildit_for_device.sh
cp simple $STAGING_DIR
cp src/appinfo.json $STAGING_DIR
echo "filemode.755=simple" > $STAGING_DIR/package.properties
palm-package $STAGING_DIR
