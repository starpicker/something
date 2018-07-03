if [ $# -eq 1 ]
then
    path="$1"
else
	echo "need jpgfolder"
	exit
fi

export LD_LIBRARY_PATH=.

for i in `ls $path/*Auto*`
do
	# width=`file $i |cut -d ' ' -f22|cut -d',' -f1 |cut -d'x' -f1`
	# height=`file $i |cut -d ' ' -f22|cut -d',' -f1 |cut -d'x' -f2`
	width=`./hdrd_test $i | cut -d" " -f2|cut -d',' -f1`
	height=`./hdrd_test $i | cut -d" " -f4|cut -d',' -f1`
	# echo $width $height
	orientation=0
	if [ $width -lt $height ];then
			orientation=0
		else
			orientation=270
	fi
	# echo $orientation
	conval=`./hdrd_test $i $orientation |sed -n '2p'|cut -d' ' -f6|cut -d ',' -f1`
	
	newname=$path/`basename $i .jpg`_${conval}_${orientation}.jpg
	# echo $newname
	echo "mv $i $newname"
	mv $i $newname
done