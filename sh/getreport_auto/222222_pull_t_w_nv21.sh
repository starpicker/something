#!/bin/sh
adb root
adb remount

wildname1='tele*'
wildname2='wide*'

mkdir -p nv21

if [ $# -eq 1 ]
then
    wildname="*$1*"
fi

#echo $wildname

for i in `adb shell "while true;do ls /data/misc/camera/$wildname1.nv21;break;done"`;do adb pull $i nv21;done
for i in `adb shell "while true;do ls /data/misc/camera/$wildname2.nv21;break;done"`;do adb pull $i nv21;done
#pause
