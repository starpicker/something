echo "ls nv21"
./111111_ls_nv21.sh
#pause

echo "pull wide and tele"
./222222_pull_t_w_nv21.sh
#pause

echo "crop nv21"
./333333_crop_nv21.py
#pause

echo "convert nv21 to jpg"
./444444_convert.sh
#pause

echo "calculate CA"
./555555_Average_CA.py
echo "check conclusion/whitebalance.txt"
#pause

echo "calculate lumi"
./666666_Average_lumi.py
echo "check conclusion/brightness.txt"
pause
