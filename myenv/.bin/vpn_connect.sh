#!/usr/bin/expect
#set timeout 20 #设置超时时间
spawn /opt/cisco/anyconnect/bin/vpn connect xxxxxxxxxx
expect "*Connect Anyway?*"
send "y\r"
expect "*Group: *"
send "\r"
expect "*xxxxxxxxxxxxxx.com.cn*"
send "\r"
expect "Password:*"
send "xxxxxxx\r"
interact
#expect "*VPN>"
#send "q\r"
