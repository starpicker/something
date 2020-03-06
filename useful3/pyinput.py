# mouse control
from  pynput.mouse import Button, Controller
import time 

mouse = Controller()
print(mouse.position)
time.sleep(3)
print('The current pointer position is {0}'.format(mouse.position))


#set pointer positon
mouse.position = (277, 645)
print('now we have moved it to {0}'.format(mouse.position))

#鼠标移动（x,y）个距离
#param int x: The horizontal offset.
#param int dy: The vertical offset.
mouse.move(5, -5)
print(mouse.position)

mouse.press(Button.left)
mouse.release(Button.left)

mouse.press(Button.right)
mouse.release(Button.right)

#Double click
#param int count: The number of clicks to send.
mouse.click(Button.left, 2)

#scroll two     steps down
#param int dx: The horizontal scroll. 
#param int dy: The vertical scroll.
mouse.scroll(0, 500)


# mouse listen
'''
:param callable on_move: The callback to call when mouse move events occur.

        It will be called with the arguments ``(x, y)``, which is the new
        pointer position. If this callback raises :class:`StopException` or
        returns ``False``, the listener is stopped.

    :param callable on_click: The callback to call when a mouse button is
        clicked.

        It will be called with the arguments ``(x, y, button, pressed)``,
        where ``(x, y)`` is the new pointer position, ``button`` is one of the
        :class:`Button` values and ``pressed`` is whether the button was
        pressed.

        If this callback raises :class:`StopException` or returns ``False``,
        the listener is stopped.

    :param callable on_scroll: The callback to call when mouse scroll
        events occur.

        It will be called with the arguments ``(x, y, dx, dy)``, where
        ``(x, y)`` is the new pointer position, and ``(dx, dy)`` is the scroll
        vector.

        If this callback raises :class:`StopException` or returns ``False``,
        the listener is stopped.

    :param bool suppress: Whether to suppress events. Setting this to ``True``
        will prevent the input events from being passed to the rest of the
        system.
'''

from pynput import mouse
from  pynput.mouse import Button

def on_move(x, y):
    print('Pointer moved to {o}'.format((x,y)))

def on_click(x, y , button, pressed):
    button_name = ''
    #print(button)
    if button == Button.left:
        button_name = 'Left Button'
    elif button == Button.middle:
        button_name = 'Middle Button'
    elif button == Button.right:
        button_name = 'Right Button'
    else:
        button_name = 'Unknown'
    if pressed:
        print('{0} Pressed at {1} at {2}'.format(button_name, x, y))
    else:
        print('{0} Released at {1} at {2}'.format(button_name, x, y))
    if not pressed:
        return False

def on_scroll(x, y ,dx, dy):
    print('scrolled {0} at {1}'.format(
        'down' if dy < 0 else 'up',
        (x, y)))

while True:
    with mouse.Listener( no_move = on_move,on_click = on_click,on_scroll = on_scroll,suppress = False) as listener:
        listener.join()


#keyboard control
'''
['alt', 'alt_l', 'alt_r', 'backspace', 'caps_lock', 'cmd', 'cmd_r', 'ctrl', 'ctrl_l', 'ctrl_r', 'delete', 
'down', 'end', 'enter', 'esc', 'f1', 'f10', 'f11', 'f12', 'f13', 'f14', 'f15', 'f16', 'f17', 'f18', 'f19', 'f2', 'f20', 
'f3', 'f4', 'f5', 'f6', 'f7', 'f8', 'f9', 'home', 'insert', 'left', 'menu', 'num_lock', 'page_down', 'page_up', 'pause',
'print_screen', 'right', 'scroll_lock', 'shift', 'shift_r', 'space', 'tab', 'up']
'''

from pynput.keyboard import Key, Controller

keyboard = Controller()

#Press and release space
keyboard.press(Key.space)
keyboard.release(Key.space)

#Type a lower case A ;this will work even if no key on the physical keyboard  is labelled 'A'
keyboard.press('a')
keyboard.release('a')

#Type two  upper case As
keyboard.press('A')
keyboard.release('A')
# or 
#Executes a block with some keys pressed.    param keys: The keys to keep pressed.
with keyboard.pressed(Key.shift):    #组合按键
    keyboard.press('a')
    keyboard.release('a')

#type 'hello world '  using the shortcut type  method
#This method will send all key presses and releases necessary to type all characters in the string.
#param str string: The string to type.
keyboard.type('hello world')

keyboard.touch('&', True)
keyboard.touch('&', False)
    
keyboard.press(Key.print_screen)
keyboard.release(Key.print_screen)

with keyboard.pressed(Key.ctrl):    #组合按键
    keyboard.press('s')
    keyboard.release('s')

#keyboard listen
from pynput import keyboard

#alt_pressed、alt_gr_pressed、ctrl_pressed、shift_pressed


def on_press(key):
    try:
        print('alphanumeric key     {0} pressed'.format(key.char))    #应该记录下之前有没有ctrl、alt、和shift按下
    except AttributeError:
        print('special key {0} pressed'.format(key))

def on_release(key):
    print('{0} released'.format(key))
    if key == keyboard.Key.esc:
        return False

while True:
    with keyboard.Listener(
        on_press = on_press,
        on_release = on_release,
        suppress = False) as listener:
        listener.join()
