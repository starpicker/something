envpath='/home/lma'
homepath='/home/ml'
binpath='.env/usr/bin'
libpath='.env/usr/lib'

sudo cp -r $envpath/.bin       $homepath
sudo cp -r $envpath/.vim*      $homepath
sudo cp -r $envpath/.tmux*     $homepath
sudo cp $envpath/.bash*        $homepath
sudo cp $envpath/.bash*        $homepath
sudo cp $envpath/.profile      $homepath
sudo cp $envpath/.commacd.bash $homepath
sudo cp $envpath/.fzf.*        $homepath
sudo cp $envpath/.git*         $homepath

sudo chown -R ml:ml   $homepath

sudo mkdir -p $homepath/$binpath
sudo mkdir -p $homepath/$libpath

sudo rsync -rtopgDzv /usr/bin/* $homepath/$binpath/
sudo rsync -rtopgDzv /bin/* $homepath/$binpath/
sudo rsync -rtopgDzvl /lib/x86_64-linux-gnu/* $homepath/$libpath/
sudo rsync -rtopgDzvl /usr/lib/gcc $homepath/$libpath/

sudo cp /usr/bin/vim.basic $homepath/$binpath/vim
sudo cp /usr/bin/gawk $homepath/$binpath/awk
sudo cp /usr/lib/android-sdk/platform-tools/adb $homepath/$binpath/adb
sudo cp /usr/lib/android-sdk/platform-tools/fastboot $homepath/$binpath/fastboot
sudo ln -s $homepath/$binpath/x86_64-linux-gnu-g++-7 $homepath/$binpath/g++
sudo ln -s $homepath/$binpath/x86_64-linux-gnu-gcc-7 $homepath/$binpath/gcc
sudo ln -s $homepath/$binpath/x86_64-linux-gnu-cpp-7 $homepath/$binpath/cpp
sudo ln -s $homepath/$binpath/x86_64-linux-gnu-readelf $homepath/$binpath/readelf
sudo ln -s $homepath/$binpath/x86_64-linux-gnu-objdump $homepath/$binpath/objdump
sudo ln -s $homepath/$binpath/x86_64-linux-gnu-strings $homepath/$binpath/strings
sudo ln -s $homepath/$binpath/x86_64-linux-gnu-c++filt $homepath/$binpath/c++filt
sudo ln -s $homepath/$binpath/x86_64-linux-gnu-addr2line $homepath/$binpath/addr2line
sudo ln -s $homepath/$binpath/x86_64-linux-gnu-gcc-ranlib-7 $homepath/$binpath/gcc-ranlib-7
sudo ln -s $homepath/$binpath/x86_64-linux-gnu-nm $homepath/$binpath/nm
sudo ln -s $homepath/$binpath/ctags-exuberant $homepath/$binpath/ctags
sudo ln -s $homepath/$binpath/convert-im6.q16 $homepath/$binpath/convert
sudo ln -s $homepath/$binpath/identify-im6.q16 $homepath/$binpath/identify

sudo chmod a+w $homepath/.bashrc
echo "export LD_LIBRARY_PATH=.:$homepath/$libpath:$LD_LIBRARY_PATH:/usr/lib/x86_64-linux-gnu:/usr/local/lib/:/usr/local/lib64/" >> $homepath/.bashrc
