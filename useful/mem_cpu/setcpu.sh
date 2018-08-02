adb root
adb wait-for-device
adb shell "echo userspace > /sys/devices/system/cpu/cpu6/cpufreq/scaling_governor"
adb shell "echo 1843200 > /sys/devices/system/cpu/cpu6/cpufreq/scaling_setspeed"
adb shell "echo userspace > /sys/devices/system/cpu/cpu7/cpufreq/scaling_governor"
adb shell "echo 1843200 > /sys/devices/system/cpu/cpu7/cpufreq/scaling_setspeed"
adb shell "echo userspace > /sys/devices/system/cpu/cpu4/cpufreq/scaling_governor"
adb shell "echo 1843200 > /sys/devices/system/cpu/cpu4/cpufreq/scaling_setspeed"
adb shell "echo userspace > /sys/devices/system/cpu/cpu5/cpufreq/scaling_governor"
adb shell "echo 1843200 > /sys/devices/system/cpu/cpu5/cpufreq/scaling_setspeed"
pause