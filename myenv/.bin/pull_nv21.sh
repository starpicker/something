#!/bin/sh
adb root
adb remount

wildname='*'

if [ $# -eq 1 ]
then
    wildname="*$1*"
fi

echo $wildname

for i in `adb shell "while true;do ls /data/misc/camera/$wildname.nv21;break;done"`;do adb pull $i .;done
pause
