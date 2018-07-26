if [ $# != 6 ]
then
	echo "usage: $0 input.nv21 output.nv21 left top right bottom"
	exit
else
	in=$1
	out=$2
	left=$3
	top=$4
	right=$5
	bottom=$6
fi

width=`get_width $in`
height=`get_height $in`

nv21_crop $in $out $width $height $left $top $right $bottom
