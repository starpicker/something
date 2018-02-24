BAK_DIR=$(date +%Y%m%d)
mkdir -p mkdir -p /sdcard/images/faceks/$BAK_DIR

XARGS="busybox xargs"
WGET="busybox wget"

cat jpeg_urls.txt | $XARGS $WGET -c -t10 -T15 -P /sdcard/images/faceks/$BAK_DIR
