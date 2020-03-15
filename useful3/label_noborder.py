from tkinter import * 
 
root = Tk()
root.attributes("-alpha", 0.8)

class MyFrame(Frame):
    def __init__(self,master=root, *args, **kwargs):
        Frame.__init__(self,master, *args, **kwargs)
        self.grid()
        self.top = self.winfo_toplevel()
        self.top.update_idletasks()
        self.top.overrideredirect(True)

frame = MyFrame()

l1= Label(frame, text='F:打印机故障', bg="seagreen", font=("Arial", 22))
l1['width'] = 60
l1['height'] = 4
l1.pack()

l2= Label(frame, text='C:电脑故障', bg="burlywood", font=("Arial", 22))
l2['width'] = 60
l2['height'] = 4
l2.pack()

l3= Label(frame, text='S:软件故障', bg="lightpink", font=("Arial", 22))
l3['width'] = 60
l3['height'] = 4
l3.pack()

def func(event):
    print("event.char =", event.char)
    if event.char == 'q':
        root.quit()
 
root.bind("<Key>", func)

root.mainloop()
