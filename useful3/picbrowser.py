#!/usr/bin/env Python
#coding=utf-8

#filename : PictureBrowser.py
#author   : http://www.cnblogs.com/chenkun24/
#date     : 2012-10-11



import wx
import os
import sys
import string

#你有H盘吗？没有的话在这个初始化函数里修改加载的初始路径
class PBDirFrame(wx.Frame):
    def __init__(self, app):
        wx.Frame.__init__(self, None, -1, "选择文件夹", size=(250,500))

        self.app = app

        #设置字体
        font = wx.Font(12, wx.DEFAULT, wx.NORMAL, wx.NORMAL, False, 'Courier New')
        self.SetFont(font)
        
        #文件夹listbox
        self.list = wx.ListBox(self, -1, (0,0), (200,600), [], wx.LB_SINGLE)
        self.list.Bind(wx.EVT_LISTBOX_DCLICK, self.OnDClick)
        
        #加载当前文件夹
        curdir = os.getcwd()#在这里修改初始路径，这个是当前工作路径
        #curdir = 'H:\\'
        os.chdir(curdir)
        self.LoadDir(curdir)
        
        #绑定事件
        self.Bind(wx.EVT_CLOSE, self.OnClose)


        
        #显示窗口
        self.Show()
    
    def OnClose(self, event):
        self.Destroy()
        self.app.Close()
    
    #listbox双击事件
    def OnDClick(self, event):
        if self.list.GetSelection()==0:#判断是否选择了返回上一层文件夹
            path = os.getcwd()
            pathinfo = os.path.split(path)
            dir = pathinfo[0]
        else:#获得需要进入的下一层文件夹
            dir = self.list.GetStringSelection()
        
        if os.path.isdir(dir):#进入文件夹
            self.LoadDir(dir)
        elif os.path.splitext(dir)[-1]=='.jpg':#显示图片
            self.app.ShowImage(dir)

    #加载文件夹，如果你想定义自己的排序，那么修改这个方法吧~
    def LoadDir(self, dir):
        #不是目录则不进行操作
        if not os.path.isdir(dir):
            return
        
        self.list.Clear()#清空
        self.list.Append('...')#添加返回上一层文件夹标志

        dirs = []
        jpgs = []
        nnjpgs = []
        for _dir in os.listdir(dir):
            if os.path.isdir(dir+os.path.sep+_dir):
                dirs.append(_dir)
            else:
                info = os.path.splitext(_dir)
                if info[-1]=='.jpg':
                    if info[0].isdigit():
                        jpgs.append(string.atoi(info[0]))#转化为数字
                    else:
                        nnjpgs.append(_dir)
        jpgs.sort()
        for _jpgs in jpgs:
            self.list.Append(str(_jpgs)+'.jpg')
        for _nnjpgs in nnjpgs:
            self.list.Append(_nnjpgs)
        for _dirs in dirs:
            self.list.Append(_dirs)

        os.chdir(dir)#设置工作路径

    #获得下一张要显示的图片
    def GetNextImage(self):
        index = self.list.GetSelection()
        i = index
        while i+1<self.list.GetCount():
            i += 1
            if os.path.splitext(self.list.GetString(i))[-1]=='.jpg':
                break
        if i<self.list.GetCount():
            index = i
        self.list.SetSelection(index)
        return self.list.GetStringSelection()

    #获得上一张图片
    def GetPreImage(self):
        index = self.list.GetSelection()
        i = index
        while i-1>0:
            i -= 1
            if os.path.splitext(self.list.GetString(i))[-1]=='.jpg':
                break
        if i>0:
            index = i
        
        self.list.SetSelection(index)
        return self.list.GetStringSelection()


class PBPicFrame(wx.Frame):

    max_width = 400
    max_height = 600

    def __init__(self, app):
        wx.Frame.__init__(self, None, -1, "显示图片", size=(400,400))#, style=wx.SIMPLE_BORDER)

        #是否要移动图片的标志
        self.bmoved = False
        
        self.app = app

        #staticbitmap
        self.bmp = wx.StaticBitmap(self, 0, wx.NullBitmap, (0,0), (400,400))


        self.Bind(wx.EVT_MOUSEWHEEL, self.OnChangeImage)
        self.bmp.Bind(wx.EVT_LEFT_DOWN, self.OnLeftDown)
        self.bmp.Bind(wx.EVT_LEFT_UP, self.OnLeftUp)
        self.bmp.Bind(wx.EVT_MOTION, self.OnMotion)
        self.Bind(wx.EVT_KEY_DOWN, self.OnKeyDown)
        
        self.ShowFullScreen(True, style=wx.FULLSCREEN_ALL)
        self.Hide()


    def ShowImage(self, path):
        if os.path.splitext(path)[-1]!='.jpg':
            return
        self.bmppath = path
        image = wx.Image(path, wx.BITMAP_TYPE_JPEG)
        bmp = image.ConvertToBitmap()
        size = self.GetSize(bmp)
        bmp = image.Scale(size[0], size[1]).ConvertToBitmap()
        self.bmp.SetSize(size)
        self.bmp.SetBitmap(bmp)
        self.Show()

    def GetSize(self, bmp):
        width = bmp.GetWidth()
        height = bmp.GetHeight()
        if width>self.max_width:
            height = height*self.max_width/width
            width = self.max_width
        if height>self.max_height:
            width = width*self.max_height/height
            height = self.max_height
        size = width, height
        return size
        

    def OnChangeImage(self, event):
        rotation = event.GetWheelRotation()
        if rotation<0:
            self.app.ShowNextImage()
        else:
            self.app.ShowPreImage()
    
    def OnLeftDown(self, event):
        self.pos = event.GetX(), event.GetY()
        self.bmoved = True

    def OnLeftUp(self, event):
        self.bmoved = False

    def OnMotion(self, event):
        if not self.bmoved:
            return
        pos = event.GetX(), event.GetY()
        dx = pos[0]-self.pos[0]
        dy = pos[1]-self.pos[1]
        pos = self.bmp.GetPosition()
        pos = pos[0]+dx, pos[1]+dy
        self.bmp.SetPosition(pos)

    def OnKeyDown(self, event):
        keycode = event.GetKeyCode()
        if keycode == 49:#数字1放大
            self.SizeUp()
        elif keycode == 50:#数字2缩小
            self.SizeDown()
        event.Skip()#这个貌似很重要，要同时触发app上的快捷键

    def SizeUp(self):
        self.max_width += 50
        self.max_height += 75
        self.ShowImage(self.bmppath)
    def SizeDown(self):
        self.max_width -= 50
        self.max_height -= 75
        self.ShowImage(self.bmppath)

class PBApp(wx.App):
    
    #redirect=False将信息输出到dos界面
    def __init__(self, redirect=False):
        wx.App.__init__(self, redirect)
    
    def OnInit(self):
        
        #显示文件夹列表界面
        self.dirframe = PBDirFrame(self)
        
        #显示图片界面
        self.picframe = PBPicFrame(self)
        
        #绑定事件
        self.Bind(wx.EVT_KEY_DOWN, self.OnKeyDown)
        return True

    def ShowImage(self, path):
        #print 'showing app img', path
        self.picframe.ShowImage(path)
        self.picframe.SetFocus()
    
    def ShowNextImage(self):
        path = self.dirframe.GetNextImage()
        self.ShowImage(path)

    def ShowPreImage(self):
        path = self.dirframe.GetPreImage()
        self.ShowImage(path)

    def OnKeyDown(self, event):
        keycode = event.GetKeyCode()
        #print keycode
        if keycode == 27:# ESC键
            #切换图片窗体的显示和隐藏
            if self.picframe.IsShown():
                self.picframe.Hide()
            else:
                self.picframe.Show()
    
    def Close(self):
        self.picframe.Close()
    
    
def main():
    app = PBApp()
    app.MainLoop()

if __name__=='__main__':
    main()
