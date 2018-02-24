if [ $# -lt 1 ]
then
	echo "usage: $0 input [width] [height]"
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

	tempout_y=/tmp/out.y
	tempout_uv=/tmp/out.uv
	tempout=/tmp/temp.yuv

	yoff=`echo $width*$height|bc`

	dd if=$in of=$tempout_y ibs=1 count=$yoff > /dev/null  2>&1
	dd if=$in of=$tempout_uv ibs=1 conv=swab skip=$yoff > /dev/null 2>&1
	cat $tempout_y $tempout_uv > $tempout

	#vooya --width $width --height $height --color yuv --packing nv12 $tempout

	outname=/tmp/${width}x${height}.nv12
	mv $tempout $outname

	vooyar $outname
fi
