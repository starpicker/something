inputpath=temp_out
inputdata=input.data

for i in `ls $inputpath`
do
    file=$inputpath/$i
    ./write_grey2txt_append $file $inputdata 0
    ./tinn $inputdata $file
done
