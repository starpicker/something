if [ $# -lt 4 ]
then
	echo "usage: $0 input.jpg output.jpg x y"
	exit
else
	in=$1
	out=$2
	x=$3
	y=$4
fi

if [ $# -ge 5 ]
then
	rectwidth=$5
else
	rectwidth=5
fi

if [ $# -eq 6 ]
then
	linewidth=$6
else
	linewidth=5
fi

left=$(($x-$rectwidth))
top=$(($y-$rectwidth))
right=$(($x+$rectwidth))
bottom=$(($y+$rectwidth))

./draw_rect_jpg.sh $in $out $left $top $right $bottom $linewidth
