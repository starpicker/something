#!/bin/sh
adb root
adb remount

wildname='*'

if [ $# -eq 1 ]
then
    wildname="*$1*"
fi

adb shell rm /sdcard/DCIM/Camera/$wildname.jpg
pause
