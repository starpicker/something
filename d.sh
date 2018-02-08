mv wrap wrapc
git checkout wrap.zip
unzip warp.zip
diff -upraN wrap/ wrapc/ > wrap.diff
zip -e wrap.zip wrap.diff