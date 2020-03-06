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
	#with mouse.Listener( no_move = on_move,on_click = on_click,on_scroll = on_scroll,suppress = False) as listener:
    with mouse.Listener(on_click = on_click, suppress = False) as listener:
        listener.join()
