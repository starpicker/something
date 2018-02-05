rmdir /s /q arcsoftdump

adb pull /sdcard/DCIM/arcsoftdump .

adb shell rm /sdcard/DCIM/arcsoftdump/*

pause