import pyautogui

pyautogui.PAUSE = 0.5
pyautogui.FAILSAFE = True
pyautogui.hotkey('winleft', 'e')
pyautogui.hotkey('alt', 'd')
pyautogui.typewrite('ftp://0.0.0.0:3001/xxxxx/')
pyautogui.hotkey('enter')
pyautogui.typewrite('administrator')
pyautogui.hotkey('tab')
pyautogui.typewrite('xxxxxxxx')
pyautogui.hotkey('enter')
