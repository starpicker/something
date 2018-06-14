#!/bin/bash
if [ $# != 1 ]
then
	echo "missing path"
	exit
fi

path=$1
tagpath=''

while [ $path != "/" ]
do
	if [ -f "$path/tags" ]
	then
		tagpath=$path
		break
	fi

	path=`dirname $path`
done

if [ -z "$tagpath" ]
then
	echo "no tags or git"
	exit
fi

find $tagpath -type f -newer $path/tags -a \( -name "*.h" -o -name "*.cpp" -o -name "*.c" -o -name "*.cc" -o -name "*.hpp" -o -name "*.cxx" \) > $tagpath/ctags.files

ctags -a --c++-kinds=+px --fields=+iaS --extra=+q -L $tagpath/ctags.files -f $path/tags
rm $tagpath/ctags.files

pause
