SITEMAP=sitemap_$(date +%Y%m%d).txt
IMAGES_DAYLY_LIST=images_dayly_list_$(date +%Y%m%d).txt
IMAGES_URL="http://www.mzitu.com/sitemap.xml"
LIST_PATTEN="http://.*pt-post.*xml"
LIST__REVERSE_PATTEN="http://www.*misc.*xml|http://www.*tax-category.*xml|http://www.*pt-page.*xml|http://www.*archives.*xml"
TOTAL_LIST=total_list.txt
ADD_LIST=add_$(date +%Y%m%d).txt
#IMAGES_PAGE_URL_PATTEN="img alt"
IMAGES_URL_PATTEN="http://.*jpg"
BAK_DIR=$(date +%Y%m%d)
IMAGE_TOTAL_LIST_ADDED=image_total_list_added.txt
ADD_LIST_TWO=add_list_two.txt


touch $TOTAL_LIST

curl --retry 10 --retry-max-time 15 $IMAGES_URL  > $SITEMAP
cp $SITEMAP sitemap_last.xml
grep -vE $LIST__REVERSE_PATTEN  $SITEMAP | grep -o $LIST_PATTEN > $IMAGES_DAYLY_LIST

diff $IMAGES_DAYLY_LIST $TOTAL_LIST | grep "<" | cut -d" " -f2 > $ADD_LIST 

cat $ADD_LIST >> $TOTAL_LIST

nouse(){
for i in `cat $ADD_LIST`; do 
#NUM=`curl $i | grep "var totalpage" | awk 'BEGIN{RS=";"}{print $4}' | cut -d"<" -f1 | sed '/^ *$/d'`
NUM=`curl $i | iconv -f gb2312 -t utf-8 2>/dev/null | grep -o "共.*页"|cut -d":" -f1|grep -o "[0-9]*[0-9]"`

echo ${i} >> $IMAGE_TOTAL_LIST_ADDED

for ((j=2;j<NUM;j++));do
PREFIX=`echo $i | awk 'BEGIN{FS=".html"}{print $1}'`
echo ${PREFIX}_${j}.html >> $IMAGE_TOTAL_LIST_ADDED
done

done
}

for i in `cat $ADD_LIST`; do 
curl $i | grep -o "http://www.mzitu.com.*" | cut -d"<" -f1 | sed '/sitemap/d'>> $ADD_LIST_TWO
done


for i in `cat $ADD_LIST_TWO`; do 

curl $i | grep "img src" | awk 'BEGIN{RS="img src="}{print $1}'|sed '1d'| grep -o $IMAGES_URL_PATTEN | xargs wget -t10 -T15
#get images NUM
NUM=`curl $i | grep -o "$i/[2-9][2-9]*" | awk 'BEGIN{FS="/";MAX=0}{if($NF+0>MAX+0) MAX=$NF fi}END{print MAX}'`

for((j=2;j<NUM;j++));do
	echo "$i/$j" >> $IMAGE_TOTAL_LIST_ADDED
done

done

cat $IMAGE_TOTAL_LIST_ADDED | xargs curl | grep "img src" | awk 'BEGIN{RS="img src="}{print $1}'|sed '1d'| grep -o $IMAGES_URL_PATTEN | xargs wget -t10 -T15

mkdir $BAK_DIR
mv $SITEMAP $BAK_DIR
mv $IMAGES_DAYLY_LIST $BAK_DIR
mv $ADD_LIST $BAK_DIR
mv *jpg $BAK_DIR
rm $IMAGE_TOTAL_LIST_ADDED
rm $ADD_LIST_TWO