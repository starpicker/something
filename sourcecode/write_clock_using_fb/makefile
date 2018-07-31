SYSROOT_PATH    :=/opt/poky/1.7/sysroots/cortexa9hf-vfp-neon-poky-linux-gnueabi
OPTION          := -march=armv7-a -mthumb-interwork -mfloat-abi=hard -mfpu=neon -mtune=cortex-a9 --sysroot=$(SYSROOT_PATH)
TC_VERSION      := 1.7
CROSS           := /opt/poky/$(TC_VERSION)/sysroots/i686-pokysdk-linux/usr/bin/arm-poky-linux-gnueabi/arm-poky-linux-gnueabi-
CXX             := $(CROSS)g++ $(OPTION) -std=c++11
CC             := $(CROSS)gcc $(OPTION)

CXXFLAGS        += -g -DLINUX -fPIC -I. -L. -lturbojpeg

target          := fbclock

$(target): $(wildcard *.c)
	$(CXX) -I. *.c -lm -o $@

clean:
	-rm -f $(target)
