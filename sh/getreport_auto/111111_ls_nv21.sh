#!/bin/sh
adb root
adb remount

wildname='*'

if [ $# -eq 1 ]
then
    wildname="*$1*"
fi

adb shell ls /data/misc/camera/$wildname.nv21
#pause
