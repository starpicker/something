SITEMAP=sitemap_$(date +%Y%m%d).txt
IMAGES_DAYLY_LIST=images_dayly_list_$(date +%Y%m%d).txt
IMAGES_URL="http://mmonly.com/sitemap.xml"
LIST_PATTEN="http://.*html"
TOTAL_LIST=total_list.txt
ADD_LIST=add_$(date +%Y%m%d).txt
#IMAGES_PAGE_URL_PATTEN="img alt"
IMAGES_URL_PATTEN="http://.*jpg"
BAK_DIR=$(date +%Y%m%d)
IMAGE_TOTAL_LIST_ADDED=image_total_list_added.txt

touch $TOTAL_LIST

curl --retry 10 --retry-max-time 15 $IMAGES_URL  > $SITEMAP
cp $SITEMAP sitemap_last.xml
grep -o $LIST_PATTEN $SITEMAP > $IMAGES_DAYLY_LIST

diff $IMAGES_DAYLY_LIST $TOTAL_LIST | grep "<" | cut -d" " -f2 > $ADD_LIST 

cat $ADD_LIST >> $TOTAL_LIST

for i in `cat $ADD_LIST`; do 
#NUM=`curl $i | grep "var totalpage" | awk 'BEGIN{RS=";"}{print $4}' | cut -d"<" -f1 | sed '/^ *$/d'`
NUM=`curl $i | iconv -f gb2312 -t utf-8 2>/dev/null | grep -o "共.*页"|cut -d":" -f1|grep -o "[0-9]*[0-9]"`

echo ${i} >> $IMAGE_TOTAL_LIST_ADDED

for ((j=2;j<NUM;j++));do
PREFIX=`echo $i | awk 'BEGIN{FS=".html"}{print $1}'`
echo ${PREFIX}_${j}.html >> $IMAGE_TOTAL_LIST_ADDED
done

done

cat $IMAGE_TOTAL_LIST_ADDED | xargs curl | grep "img alt" | grep -o $IMAGES_URL_PATTEN | xargs wget -t10 -T15

mkdir $BAK_DIR
mv $SITEMAP $BAK_DIR
mv $IMAGES_DAYLY_LIST $BAK_DIR
mv $ADD_LIST $BAK_DIR
mv *jpg $BAK_DIR
rm $IMAGE_TOTAL_LIST_ADDED