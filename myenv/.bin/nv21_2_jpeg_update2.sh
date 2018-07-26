mkdir -p /home/arcsoft/Desktop/dump/jpg

if [ $# -lt 1 ]
then
	echo "usage: $0 input.nv21 [output.jpg] [width] [height]"
	exit
else
	in=$1

	if [ $# != 5 ]
	then
		width=`get_width $in`
		height=`get_height $in`
		outname=`get_jpg_name $in`
	else
		outname=$2
		width=$3
		height=$4
	fi

	keyname=`basename $in .nv21`
	keyname=`basename $keyname .yuv`

	tempout=/tmp/$keyname.rgb32
	nv21_to_rgb32 $in $tempout $width $height

	#if [ $# != 5 ]
	#then
		#outname=$keyname.jpg
	#fi

	encode_jpeg $tempout "/home/arcsoft/Desktop/dump/jpg/"$outname $width $height
fi
