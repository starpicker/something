if [ $# -lt 1 ]
then
	echo "usage: $0 input.nv21 [width] [height]"
	exit
else
	in=$1

	if [ $# != 4 ]
	then
		width=`get_width $in`
		height=`get_height $in`
	else
		width=$2
		height=$3
	fi

	keyname=`basename $in .nv21`
	keyname=`basename $keyname .yuv`

	tempout=/tmp/$keyname.rgb32
	nv21_to_rgb32 $in $tempout $width $height

	outname=$keyname.jpg
	encode_jpeg $tempout $outname $width $height
fi

pause
