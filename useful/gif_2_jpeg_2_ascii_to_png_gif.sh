#!/bin/sh

if [ $# -ne 1 ]
then
echo "\033[;31musage: $0   giffile\033[0m"
exit
fi

#gif2jpg //var cannot have space behind it
PREFIX=`echo $1 | awk -F. '{print $1}'`
echo "mkdir $PREFIX"
mkdir $PREFIX
convert $1 $PREFIX/$PREFIX.jpeg
#will produce lots of jpegs
#then do the following

#jpg2ascii2png
for i in $PREFIX/*.jpeg;do j=`echo $i | awk -F. '{print $1}'`; jp2a $i | a2png - --output=$j.png; done

#sort the pngs
for i in $PREFIX/*.jpeg; do j=`basename $i .jpeg`;k=`echo $j | awk -F- '{print $2}'`; echo $k.png>>$PREFIX/$PREFIX.txt; done
sort -n $PREFIX/$PREFIX.txt -o $PREFIX/$PREFIX.txt
for i in `cat $PREFIX/$PREFIX.txt`; do echo $PREFIX/$PREFIX-$i >>$PREFIX/${PREFIX}_$PREFIX.txt; done

#png2gif
echo "convert -delay 0 -loop 0 $PREFIX/*.png ${PREFIX}_ascii.gif"
convert -delay 0 -loop 0 @$PREFIX/${PREFIX}_$PREFIX.txt ${PREFIX}_ascii.gif
echo "rm -rf $PREFIX"
rm -rf $PREFIX
