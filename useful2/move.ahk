name = 张泽
!,::

send, {tab}
sleep 100
send, {tab}
sleep 100
send, {tab}
sleep 100
clipboard=无
send ^v
sleep 200
send, {tab}
sleep 100
send, {tab}
sleep 100
send, {tab}
sleep 100
clipboard:=name
send ^v
return

!/::
send, {tab}
sleep 100
send, {tab}
sleep 100
send, {tab}
sleep 100
clipboard=一般
send ^v
sleep 200
send, {tab}
sleep 100
send, {tab}
sleep 100
send, {tab}
sleep 100
clipboard:=name
send ^v
sleep 400
send +{tab}
sleep 100
send +{tab}
sleep 100
clipboard=丹参
send ^v
send {f2}
;Click, 2
;sleep 200
;send, {tab}
;clipboard=16
;send ^v
return

!.::

send, {tab}
sleep 100
send, {tab}
sleep 100
send, {tab}
sleep 100
clipboard=一般
send ^v
sleep 200
send, {tab}
sleep 100
send, {tab}
sleep 100
send, {tab}
sleep 100
clipboard:=name
send ^v
sleep 200
send +{tab}
sleep 100
send +{tab}
return