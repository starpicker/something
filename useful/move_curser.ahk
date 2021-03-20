!,::
clipboard=第一
send ^v
sleep 100
send {tab}
sleep 100
send {tab}
sleep 100
send {tab}
sleep 100
clipboard=第二
send ^v
return

!.::
clipboard=第三
send ^v
sleep 100
send {tab}
sleep 100
send {tab}
sleep 100
send {tab}
sleep 100
clipboard=第四
send ^v
sleep 100
send +{tab}
sleep 100
;+ means shift
send +{tab}
return