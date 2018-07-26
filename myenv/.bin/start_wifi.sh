sudo killall wpa_supplicant
sudo wpa_supplicant -B -s -u -iwlp3s0 -c/etc/wpa_supplicant.conf
sudo dhclient
