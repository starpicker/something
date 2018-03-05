repo sync -j4
while [ $? -ne 0 ]
do
    repo sync -j4
done
