
TC_VERSION      := x.x
SYSROOT_PATH    :=/opt/xxxxxxxxxsysroots/cortexa9hf-vfp-neon-poky-linux-gnueabi

OPTION          := -march=armv7-a -mthumb-interwork -mfloat-abi=hard -mfpu=neon -mtune=cortex-a9 --sysroot=$(SYSROOT_PATH)

CROSS           := /opt/xxxx/$(TC_VERSION)/sysroots/i686-pokysdk-linux/usr/bin/arm-poky-linux-gnueabi/arm-poky-linux-gnueabi-
CXX             := $(CROSS)g++ $(OPTION) -std=c++11
STRIP           := $(CROSS)strip

MLFLAGS         :=
COMMONFLAGS     := -DLINUX -fPIC $(MLFLAGS) -Wall -Wno-unused-variable
CXXFLAGS        = $(COMMONFLAGS) -g -DBUILD_TIME="\"$(shell date)\""
DELIVERY_FLAGS  = $(CXXFLAGS) -DNDEBUG -fvisibility=hidden -Wno-unused-value
