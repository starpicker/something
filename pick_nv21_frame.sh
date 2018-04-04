if [ $# -lt 5 ]
then
        echo "usage: $0 input.nv21 output.nv21 width height index[start from 0]"
        exit
else
		in=$1
		out=$2
		width=$3
		height=$4
		index=$5
		size=$(($height*$width*3/2))
		dd if=$in of=$out skip=$index ibs=$size count=1
fi