#!/bin/sh

while :
do

for i in `ls image` 
do
    clear
    jp2a image/$i
	sleep 300
done

done
