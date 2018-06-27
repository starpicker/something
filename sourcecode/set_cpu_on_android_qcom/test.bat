make > NUL
adb push set_cpu_test /data/misc/camera
adb shell chmod +x /data/misc/camera/set_cpu_test
adb shell /data/misc/camera/set_cpu_test 0
adb shell /data/misc/camera/set_cpu_test 1
pause