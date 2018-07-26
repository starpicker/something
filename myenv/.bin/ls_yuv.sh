#!/bin/sh
adb shell "while true;do ls /data/misc/camera/*.yuv;break;done"

pause

