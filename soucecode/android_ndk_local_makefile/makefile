NDK_ROOT    := D:/android-ndk-r14b-windows-x86_64/android-ndk-r14b


TOOLCHAIN   := $(NDK_ROOT)/toolchains/arm-linux-androideabi-4.9/prebuilt/windows-x86_64
CXX         := $(NDK_ROOT)/toolchains/llvm/prebuilt/windows-x86_64/bin/clang++.exe -gcc-toolchain $(TOOLCHAIN)

SYSROOT     := $(NDK_ROOT)/platforms/android-21/arch-arm

STLPATH     := $(NDK_ROOT)/sources/cxx-stl/gnu-libstdc++/4.9

CFLAGS      := -fpic -ffunction-sections -funwind-tables -fstack-protector-strong -Wno-invalid-command-line-argument -Wno-unused-command-line-argument -no-canonical-prefixes -fno-integrated-as -g -target armv7-none-linux-androideabi -march=armv7-a -mfloat-abi=softfp -mfpu=vfpv3-d16 -fno-exceptions -fno-rtti -marm -O2 -DNDEBUG  -mfpu=neon   -DANDROID -Wa,--noexecstack -Wformat -Werror=format-security -fpie -fexceptions -std=c++11 --sysroot $(SYSROOT)

target      := symphony_test

INC         := -I$(STLPATH)/include \
			   -I$(STLPATH)/libs/armeabi-v7a/include \
			   -I$(STLPATH)/include/backward \
			   -I./symphony/include

LIB         := -L$(STLPATH)/libs/armeabi-v7a \
			   -lgnustl_static \
			   -lgcc \
			   -lc \
			   -lm \
			   -L./symphony/lib \
			   -lsymphonypower \
			   -lsymphony-cpu

LDFLAGS     := -Wl,--gc-sections -Wl,-z,nocopyreloc --sysroot=$(SYSROOT) -Wl,-rpath-link=$(SYSROOT)/usr/lib -no-canonical-prefixes -target armv7-none-linux-androideabi -Wl,--fix-cortex-a8 -Wl,--build-id -Wl,--no-undefined -Wl,-z,noexecstack -Wl,-z,relro -Wl,-z,now -Wl,--warn-shared-textrel -Wl,--fatal-warnings -fpie -pie

$(target) : $(target).o
	$(CXX) $^ $(LDFLAGS) $(LIB) -o $@

%.o : %.cpp
	$(CXX) $(CFLAGS) $(INC) -c -o $@ $< 

clean:
	rm $(target) $(target).o