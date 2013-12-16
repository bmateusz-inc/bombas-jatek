#!/bin/sh

for i in `ls img | grep .png$`
do
	echo ">convert $i"
	convert -channel RGBA -resize 25% img/$i img_low/$i
done

for i in `ls img | grep .anim$`
do
	echo ">copy $i"
	cp img/$i img_low/$i
done
