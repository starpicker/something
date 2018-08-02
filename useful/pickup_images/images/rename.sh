#!/bin/bash

#remove space in filename
rename 's/[ ]+/_/g' $1/*

j=0

for i in `ls -1 $1`
do

mv $1/$i $1/$j.jpg
j=`expr $j + 1`

done
