#!/bin/sh

#ls img | grep .png$ > img.ls
#ls img | grep .anm$ >> img.ls
#ls img | grep .mlt$ >> img.ls
#ls img | grep .txh$ >> img.ls

echo "release"

DIST="dist/Release/MinGW-Windows"

rm -rf $DIST/img
rm -rf $DIST/img_low
rm -rf $DIST/maps

cp -r img $DIST
cp -r img_low $DIST
cp -r maps $DIST
