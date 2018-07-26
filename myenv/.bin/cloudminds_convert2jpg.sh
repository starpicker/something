crop_nv21.sh tele_4032x3000_*.nv21 tele_4000x3000.nv21 0 0 4000 3000
crop_nv21.sh wide_4032x3000_*.nv21 wide_4000x3000.nv21 0 0 4000 3000
nv21_2_jpeg_update.sh tele_4000x3000.nv21 tele_0.jpg 4000 3000
nv21_2_jpeg_update.sh wide_4000x3000.nv21 wide_1.jpg 4000 3000


rm *.nv21

pause

