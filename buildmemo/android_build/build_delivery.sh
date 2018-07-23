#!/bin/sh

#step 1: build dmr sink engine

echo
echo
echo

echo "              \033[1;32;40m1.build dmr sink engine \033[0m"

echo
echo
echo

version=`cat version.json | jq -r '.version'`
svn_version=`cd .. && svn info |grep Revision:|awk '{print $2}'`

echo "     \033[1;32;40mdmr version is $version, svn version is $svn_version  \033[0m"
echo
echo
echo

sleep 2s

cd ../build && make delivery -j4 -B MLFLAGS="-DDMR_VERSION=\\\"${version}\\\" -DSVN_VERSION=\\\"${svn_version}\\\""
cd -
cp ../input/lib/xxxxxxxxxxxx.so ./xxx_testbed/input/lib/xxxxxxxxxxxx.so
cd xxx_testbed && make -j4 MLFLAGS="-DDMR_VERSION=\\\"${version}\\\" -DSVN_VERSION=\\\"${svn_version}\\\""
cd -

echo
echo
echo

if [ $? -eq 0 ];then

	echo "              \033[1;32;40mbuilding successful, continue... \033[0m"
else
	echo "              \033[1;31;40mbuilding fail, exit \033[0m"
	exit
fi

echo
echo
echo

#step2:copy lib and inc and testbed

echo
echo
echo

echo "              \033[1;32;40m2.copy lib and inc and testbed \033[0m"

echo
echo
echo

sleep 1s

rm -rf ./output
mkdir ./output
cp -r ./input/* ./output/
find ./output/ -type d -name ".svn" -exec rm -rf {} \;

echo "cp ../output/xxxxxxxxx.h ./output/Include/xxxxxxxxx.h"
cp ../output/xxxxxxxxx.h ./output/Include/xxxxxxxxx.h

#step3:package

echo
echo
echo

echo "              \033[1;32;40m3.package the delivery \033[0m"

echo
echo
echo

sleep 1s

times=`cat version.json | jq -r '.times'`
version=`cat version.json | jq -r '.version'`
date=`date +%Y%m%d`

API_version=`cat version.json | jq -r '.API_version'`
Guide_version=`cat version.json | jq -r '.Guide_version'`

PKGNAME="#10343_D"${times}"_xxxxxxxxxxxxxxxxx"${version}"_"${date}
APIDOC="xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx_v"${API_version}".doc"
GUIDEDOC="xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxv"${Guide_version}".doc"

echo "package name is \033[1;32;40m${PKGNAME}\033[0m"
echo "api doc name is \033[1;32;40m${APIDOC}\033[0m"
echo "guide doc name is \033[1;32;40m${GUIDEDOC}\033[0m"

echo
echo
echo

mv ./output/Document/xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx.doc ./output/Document/${APIDOC}
mv ./output/Document/xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxe.doc ./output/Document/${GUIDEDOC}

cd output && zip -r ../${PKGNAME}.zip ./*
cd -

echo
echo
echo

if [ $? -eq 0 ];then

	echo "              \033[1;32;40mpackage successful, clean workspace \033[0m"
else
	echo "              \033[1;31;40mpackage successful, exit \033[0m"
	exit
fi

echo
echo
echo


#step4:clean

echo
echo
echo

echo "              \033[1;32;40m4.clean workspace \033[0m"

echo
echo
echo

sleep 1s

cd ../build && make clean
cd -
cd xxx_testbed && make clean && rm ./input/lib/libxxxxxxxxx.so
cd -

echo
echo
echo
echo "              \033[1;32;40mclean workspace done\033[0m"
echo
echo
echo

exit
