adb push hdrd_test /data/misc/camera
adb shell chmod +x /data/misc/camera/hdrd_test
adb push lib\libarcsoft_high_dynamic_range.so /data/misc/camera/
adb push for_each_do.sh /data/misc/camera/
adb shell chmod +x /data/misc/camera/for_each_do.sh
REM adb shell "cd /data/misc/camera/ && export LD_LIBRARY_PATH=. && ./hdrd_test 001_Red_HDR_Auto.jpg 0"
REM adb shell "cd /data/misc/camera/ && export LD_LIBRARY_PATH=. && ./hdrd_test 017_Red_HDR_Auto.jpg 270"
pause