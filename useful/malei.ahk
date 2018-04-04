SetCapsLockState , AlwaysOff

CapsLock::SendInput {Esc}
;Esc::CapsLock

;capslock & h::
;send {lwin down}
;send 1
;send {lwin up}
;return

;capslock & j::
;send {lwin down}
;send 2
;send {lwin up}
;return

;capslock & k::
;send {lwin down}
;send 3
;send {lwin up}
;return

;capslock & l::
;send {lwin down}
;send 4
;send {lwin up}
;return

;capslock & n::
;send {lwin down}
;send 5
;send {lwin up}
;return

;capslock & m::
;send {lwin down}
;send 6
;send {lwin up}
;return

;capslock & o::
;run "C:\ProgramData\Microsoft\Windows\Start Menu\Programs\Microsoft Office\Microsoft Outlook 2010"
;return
;
;;:://b::
;run "C:\Program Files (x86)\Google\Chrome\Application\chrome.exe" baidu.com
;return

;:://d::
;run "C:\Program Files (x86)\Google\Chrome\Application\chrome.exe" doc-server
;return

;::/o::
;run "C:\ProgramData\Microsoft\Windows\Start Menu\Programs\Microsoft Office\Microsoft Outlook 2010"
;return

;::,.::
;run "E:\hitachi_reconstruction\output\copy_so.bat"
;return

;::,,::
;run "E:\olympus\push.bat"
;return

;capslock & p::
;run cmd
;return
;
;capslock & u::
;run explorer
;return
;
;capslock & f::
;run explorer
;return

;::/e1::
;run explorer E:\hitachi_reconstruction
;return

;::/calc::
;run calc
;return

;!n:: send {end}{enter}
;!a:: send {home}+{end}
;!u:: send {pgup} 
;!i:: send {pgdn}
!h:: send {left}
!l:: send {right}
!j:: send {down}
!k:: send {up}
;!o:: send {enter}
;!p:: send {backspace}
!,:: send !{space}n
;!/:: send !{space}x
;!m:: rbutton
;!c::
;run cmd
;return

^0::
run "C:\ProgramData\Microsoft\Windows\Start Menu\Programs\Microsoft Office\Microsoft Outlook 2010"
return

;^/::
;send {lwin down}
;send 4
;send {lwin up}
;return

^,:: send !{f4}
;^m::
;send {lwin down}
;send d
;send {lwin up}
;return

^;:: send +{insert}

;^e::
;run explorer E:\hitachi_reconstruction
;return

^.::send {delete}

;esc & u::send {pgup}
;esc & i::send {pgdn}
;esc & h::send {left}
;esc & l::send {right}
;esc & j::send {down}
;esc & k::send {up}
;esc & o::send {enter}
;esc & n::send {backspace}
;esc & m::send {rbutton}

;^h::send {left}
;^l::send {right}
;^j::send {down}
;^k::send {up}
;^o::send {enter}
;^n::send {backspace}

;esc::capslock

;^+c::
;; null= 
;send ^c
;sleep,200
;clipboard=%clipboard% ;%null%
;tooltip, %clipboard%
;sleep, 500
;tooltip,
;return
;
;capslock & -::
;;SetTitleMatchMode, 2
;winactivate, A
;sleep, 500
;WinSet, Transparent, 180, A
;WinSet, Style, -0xC00000,A
;return
;;
;capslock & =::
;;SetTitleMatchMode, 2
;winactivate, A
;;sleep, 500
;WinSet, Transparent, 255, A
;WinSet, Style, +0xC00000,A
;return
;;
;capslock & 0::
;winactivate, A
;sleep, 500
;WinSet, TransColor, White, A
;return
;;
;capslock & 8::
;winactivate, A
;sleep, 500
;WinSet, TransColor, Black, A
;return
;;
;capslock & 9::
;winactivate, A
;sleep, 500
;WinSet, TransColor, off, A
;return
;;
;capslock & y::
;winactivate, A
;sleep, 500
;WinSet, AlwaysOnTop, , A
;return
;
;capslock & t::
;winactivate, A
;sleep, 500
;WinSet, AlwaysOnTop, off, A
;return

;^=::
;sleep 100
;loop
;{      
;       send 0
;       KeyWait,esc,D T1    ;;表示等待esc 在3秒内被按下
;       if (ErrorLevel ==  1)
;       {
;              continue
;       }
;       else
;       break
;}
;Return

