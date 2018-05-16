greypath=temp_out
trainingdata=temp.dat
ratefile=rate.txt

j=1

for i in `ls $greypath`
do
    path=$greypath/$i
    rate=`sed -n ${j}p $ratefile`
    #echo $path $rate
    ./write_grey2txt_append $greypath/$i $trainingdata $rate
    j=$(($j+1))
done
