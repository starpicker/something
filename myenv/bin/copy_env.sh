sudo cp -r .bin       /home/ml
sudo cp -r .vim*      /home/ml
sudo cp -r .tmux*     /home/ml
sudo cp .bash*        /home/ml
sudo cp .bash*        /home/ml
sudo cp .profile      /home/ml
sudo cp .commacd.bash /home/ml
sudo cp .fzf.*        /home/ml
sudo cp .git*         /home/ml
sudo chown -R ml:ml   /home/ml/.*
sudo rsync -rtopgDzv /usr/bin/* /home/ml/.bin/
sudo rsync -rtopgDzv /bin/* /home/ml/.bin/

sudo cp /usr/bin/vim.basic /home/ml/.bin/vim
sudo cp /usr/bin/gawk /home/ml/.bin/awk
sudo cp /usr/lib/android-sdk/platform-tools/adb /home/ml/.bin/adb
sudo cp /usr/lib/android-sdk/platform-tools/fastboot /home/ml/.bin/fastboot
sudo ln -s /home/ml/.bin/x86_64-linux-gnu-g++-7 /home/ml/.bin/g++
sudo ln -s /home/ml/.bin/x86_64-linux-gnu-gcc-7 /home/ml/.bin/gcc
sudo ln -s /home/ml/.bin/x86_64-linux-gnu-cpp-7 /home/ml/.bin/cpp
sudo ln -s /home/ml/.bin/x86_64-linux-gnu-readelf /home/ml/.bin/readelf
sudo ln -s /home/ml/.bin/x86_64-linux-gnu-objdump /home/ml/.bin/objdump
sudo ln -s /home/ml/.bin/x86_64-linux-gnu-strings /home/ml/.bin/strings
sudo ln -s /home/ml/.bin/x86_64-linux-gnu-c++filt /home/ml/.bin/c++filt
sudo ln -s /home/ml/.bin/x86_64-linux-gnu-addr2line /home/ml/.bin/addr2line
sudo ln -s /home/ml/.bin/x86_64-linux-gnu-gcc-ranlib-7 /home/ml/.bin/gcc-ranlib-7
sudo ln -s /home/ml/.bin/x86_64-linux-gnu-nm /home/ml/.bin/nm
sudo ln -s /home/ml/.bin/ctags-exuberant /home/ml/.bin/ctags
sudo ln -s /home/ml/.bin/convert-im6.q16 /home/ml/.bin/convert
sudo ln -s /home/ml/.bin/identify-im6.q16 /home/ml/.bin/identify

sudo mkdir /home/ml/.lib
sudo rsync -rtopgDzvl /lib/x86_64-linux-gnu/* /home/ml/.lib/
sudo chmod a+w /home/ml/.bashrc
echo 'export LD_LIBRARY_PATH=.:/home/ml/.lib:$LD_LIBRARY_PATH:/usr/lib/x86_64-linux-gnu:/usr/local/lib/:/usr/local/lib64/' >> /home/ml/.bashrc
