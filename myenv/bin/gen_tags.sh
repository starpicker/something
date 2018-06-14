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
	#echo $path
	if [ -f "$path/tags" ]
	then
		tagpath=$path
		break
	fi

	if [ -d "$path/.git" ]
	then
		tagpath=$path
		break
	fi
	path=`dirname $path`
done

#echo "$tagpath"

if [ -z "$tagpath" ]
then
	echo "no tags or git"
	exit
fi

rm $tagpath/cscope.in.out
rm $tagpath/cscope.po.out
rm $tagpath/cscope.out
rm $tagpath/cscope.files
rm $tagpath/tags
rm $tagpath/namespace.files
find $tagpath -name "*.h" -o -name "*.cpp" -o -name "*.c" -o -name "*.cc" -o -name "*.hpp" -o -name "*.cxx" >> $tagpath/cscope.files

ctags --c++-kinds=+px --fields=+iaS --extra=+q -L $tagpath/cscope.files -f $tagpath/tags

cscopefiles=$tagpath/cscope.files
namespacefile=$tagpath/namespace.files
namespace=namespace
substitute='//xyz123ml'

for i in `cat $cscopefiles`; do grep -l $namespace $i >> $namespacefile; done

filesize=`ls -l $namespacefile| awk '{ print $5 }'`
if [ $filesize -gt 0 ]
then
    for i in `cat $namespacefile`; do
	sed -i s#$namespace#$substitute# $i;
	chmod u+w $i;
    done
fi

cd $tagpath && cscope -bkq -i $cscopefiles

if [ $filesize -gt 0 ]
then
    for i in `cat $namespacefile`; do 
	sed -i s#$substitute#$namespace# $i;
	chmod u+w $i;
    done
fi

pause
