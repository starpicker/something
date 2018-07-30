#!/bin/bash
if [ $# != 1 ]
then
	echo "missing path"
	exit
fi

path=$1
configpath=''

while [ $path != "/" ]
do
	#echo $path

	if [ -f "$path/.syntastic_config" ]
	then
		configpath=$path
		break
	fi
	path=`dirname $path`
done

echo "let g:syntastic_cpp_config_file = \"$configpath/.syntastic_config\"" > ~/.syntastic_config_path

if [ -z "$configpath" ]
then
	echo "no .syntastic_config"
	exit
fi
