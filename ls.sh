#!/bin/sh

#ls img | grep .png$ > img.ls
#ls img | grep .anm$ >> img.ls
#ls img | grep .mlt$ >> img.ls
#ls img | grep .txh$ >> img.ls

REGEXP=".png$\|.anm$\|.mlt$\|.txh$"
ls img | grep "$REGEXP" | wc -l > img.ls
ls img | grep "$REGEXP" >> img.ls
