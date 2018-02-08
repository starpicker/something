mv wrap wrapc
git reset --hard
diff -upraN wrap/ wrapc/ > wrap.diff
zip -e wrap.diff