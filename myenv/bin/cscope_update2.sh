cscopefiles=cscope.files
namespacefile=namespace.files
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

cscope -bkq -i $cscopefiles

if [ $filesize -gt 0 ]
then
    for i in `cat $namespacefile`; do 
	sed -i s#$substitute#$namespace# $i;
	chmod u+w $i;
    done
fi
