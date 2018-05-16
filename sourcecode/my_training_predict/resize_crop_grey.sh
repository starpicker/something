outpath=temp_out

mkdir $outpath 2>/dev/null

j=0

for i in `ls jpeg`
do
    path=jpeg/$i
    outfile=$outpath/$i

    convert -resize 128x128^ $path $outfile
    convert -crop 128x128+0+0 $outfile $outfile
    jpegtran -grayscale -outfile $outfile $outfile
    keyname=`basename $outfile .jpg`
    decoder_jpeg $outfile $outpath/$keyname.grey 1>/dev/null
    rm $outfile

    j=$(($j+1))
done


#pause
