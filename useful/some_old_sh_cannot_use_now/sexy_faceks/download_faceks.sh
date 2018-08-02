SITEMAP=sitemap_$(date +%Y%m%d).txt
IMAGES_DAYLY_LIST=images_dayly_list_$(date +%Y%m%d).txt
IMAGES_URL="http://sexy.faceks.com/sitemap.xml"
LIST_PATTEN="http://.*post/.\{14\}"
TOTAL_LIST=total_list.txt
ADD_LIST=add_$(date +%Y%m%d).txt
IMAGES_PAGE_URL_PATTEN="bigimgsrc"
IMAGES_URL_PATTEN="http://imglf.*.jpg"
BAK_DIR=$(date +%Y%m%d)

touch $TOTAL_LIST

curl --retry 10 --retry-max-time 15 $IMAGES_URL  > $SITEMAP
cp $SITEMAP sitemap_last.xml
grep -o $LIST_PATTEN $SITEMAP > $IMAGES_DAYLY_LIST

diff $IMAGES_DAYLY_LIST $TOTAL_LIST | grep "<" | cut -d" " -f2 > $ADD_LIST 

cat $ADD_LIST >> $TOTAL_LIST

cat $ADD_LIST | xargs curl | grep $IMAGES_PAGE_URL_PATTEN | grep -o $IMAGES_URL_PATTEN | xargs wget -nc -t10 -T15

mkdir $BAK_DIR
mv $ADD_LIST $BAK_DIR
mv $SITEMAP $BAK_DIR
mv $IMAGES_DAYLY_LIST $BAK_DIR
mv *jpg $BAK_DIR
