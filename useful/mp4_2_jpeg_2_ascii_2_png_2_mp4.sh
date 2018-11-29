#!/bin/sh

if [ $# -ne 1 ]
then
echo "\033[;31musage: $0   mp4file\033[0m"
exit
fi

PREFIX=`echo $1 | awk -F. '{print $1}'`
echo "mkdir $PREFIX"
mkdir $PREFIX
ffmpeg -i $1 -acodec copy -vn $PREFIX/$PREFIX.aac

#max 30fps*60s*10min = 18000 means %05d
ffmpeg -i $1 -qscale:v 2 $PREFIX/$PREFIX_%05d.jpeg

#will produce lots of jpegs
#then do the following

#jpg2ascii2png
for i in $PREFIX/*.jpeg;do j=`echo $i | awk -F. '{print $1}'`; jp2a $i | a2png - --output=$j.png; done

#png2mp4
ffmpeg -i $PREFIX/$PREFIX_%05d.png  -i $PREFIX/$PREFIX.aac -vcodec h264 -acodec copy ${PREFIX}_ascii.mp4
echo "rm -rf $PREFIX"
rm -rf $PREFIX
