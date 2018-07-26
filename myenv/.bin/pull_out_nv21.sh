#!/bin/sh
adb root
adb remount

wildname='out*'

mkdir -p nv21

if [ $# -eq 1 ]
then
    wildname="*$1*"
fi

for i in `adb shell "while true;do ls /data/misc/camera/$wildname.nv21;break;done"`;do adb pull $i nv21;done
pause
