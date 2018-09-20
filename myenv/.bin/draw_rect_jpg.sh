if [ $# -lt 6 ]
then
	echo "usage: $0 input.jpg output.jpg left top right bottom"
	exit
else
	in=$1
	out=$2
	left=$3
	top=$4
	right=$5
	bottom=$6
fi

if [ $# -eq 7 ]
then
	linewidth=$7
else
	linewidth=5
fi

convert $in -fill none -stroke red -strokewidth $linewidth -draw "rectangle $left,$top $right,$bottom" $out
