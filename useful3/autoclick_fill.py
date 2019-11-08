# -*- coding: UTF-8 -*-

##import pyautogui
##import time
##print('Press Ctrl-C to quit')
##try:
##    while True:
##        x, y = pyautogui.position()
##        positionStr = 'X: {} Y: {}'.format(*[str(x).rjust(4) for x in [x, y]])
##        print(positionStr)
##        time.sleep(.5)
##except KeyboardInterrupt:
##    print('\n')

##import pyperclip
##def paste(c):
##    pyperclip.copy(c)
##    pyautogui.hotkey('ctrl', 'v')
##pyautogui.click(x=1031, y=235)
##foo = u'门诊西药房'
##paste(foo)

##
##import pyautogui
##x, y = pyautogui.locateCenterOnScreen('pyautogui/calc7key.png')
##pyautogui.click(x, y)
##list(pyautogui.locateAllOnScreen('pyautogui/calc7key.png'))

##import csv
##with open('1.csv')as f:
##    f_csv = csv.reader(f)
##    for row in f_csv:
##        print(row[0], row[1], row[2], row[3])

##import ctypes
##import time
##def DetectClick(button, watchtime = 1):
##    '''Waits watchtime seconds. Returns True on click, False otherwise'''
##    if button in (1, '1', 'l', 'L', 'left', 'Left', 'LEFT'):
##        bnum = 0x01
##    elif button in (2, '2', 'r', 'R', 'right', 'Right', 'RIGHT'):
##        bnum = 0x02
##
##    start = time.time()
##    while 1:
##        if ctypes.windll.user32.GetKeyState(bnum) not in [0, 1]:
##            # ^ this returns either 0 or 1 when button is not being held down
##            return True
##        elif time.time() - start >= watchtime:
##            break
##        time.sleep(0.001)
##    return False
##
##print('Press Ctrl-C to quit')
##try:
##    while True:
##        print(DetectClick(1))
##except KeyboardInterrupt:
##    print('\n')
