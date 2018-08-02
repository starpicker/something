#-*- coding:utf-8 –*-
#!/usr/bin/env python
#!encoding:utf-8
import os,sys,time,profile
import re

setnum = 2

PACKAGE_NAME = "com.sonymobile.addoncamera.portraitselfie"

WAIT_TIME = 0

CMD = "cmd.exe /C {0}"

procDict = dict()
topDict = dict()
num=""
# get all values id in procrank return
# get all values id in top return
def replaceAll(old, new, str):
    while str(str).ind(old) > -1:
        str = str.replace(old, new)
    return str
    
#方法0
def init_top():
    print "-------------adb shell ps -n 1------------"
    FIND_NAME="adb shell \"ps -A | grep "+PACKAGE_NAME +"\""
    ret=os.popen(CMD.format(FIND_NAME))
    #print ret.readline()
    while True:
        res = ret.readline()
        res2=re.sub(r"\s{2,}", " ",str(res))
        num=str(res2.split( )[1])
        print  "num: "+ num
        break
    return num

def topOnly0():
    num=init_top()
    write("C_size"+","+"C_alloc"+","+"C_free"+","+"JAVA_size"+","+"JAVA_alloc"+","+"JAVA_free"+","+"ALL_size"+","+"ALL_alloc_main"+","+"ALL_free"+",TIME")
    while True:
        data = getTopLine0(num)
        write(data[0]+","+data[1]+","+data[2]+","+data[3]+","+data[4]+","+data[5]+","+data[6]+","+data[7]+","+data[8]+","+getTime())
        time.sleep(WAIT_TIME)
    
def getTopLine0(num):
    tagList=[]
    FIND_NAME2="adb shell \"dumpsys meminfo "+str(num) +"\""
    ret = os.popen(CMD.format(FIND_NAME2))
    for title in ret.readlines():
        if title.find("Native Heap")!=-1 and title.find("Native Heap:") ==-1:
            title2 = re.sub(r"\s{2,}", " ",str(title)).split( )
            tagList.append(title2[6])
            tagList.append(title2[7])
            tagList.append(title2[8])
        if title.find("Dalvik Heap")!=-1 and title.find("Dalvik Heap:") ==-1:
            title2 = re.sub(r"\s{2,}", " ",str(title)).split( )
            tagList.append(title2[6])
            tagList.append(title2[7])
            tagList.append(title2[8])
        if title.find("TOTAL")!=-1 and title.find("TOTAL:") ==-1:
            title2 = re.sub(r"\s{2,}", " ",str(title)).split( )
            tagList.append(title2[5])
            tagList.append(title2[6])
            tagList.append(title2[7])
    return tagList
#方法1
def getTopLine1():
    tagList=[]
    FIND_NAME2="adb shell procrank | find \""+PACKAGE_NAME+"\""
    ret = os.popen(CMD.format(FIND_NAME2))
    title=ret.readline()
    print(title)
    title2 = re.sub(r"\s{2,}", " ",str(title)).split( )
    tagList.append(title2[0])
    tagList.append(title2[1].replace("K",""))
    tagList.append(title2[2].replace("K",""))
    tagList.append(title2[3].replace("K",""))
    tagList.append(title2[4].replace("K",""))
    return tagList

def write(context):
    report = open(sys.path[0]+"\\Report.csv","a")
    report.write(context+"\n")
    print context
    report.close()

def getTime():
    return time.strftime(time.strftime("%Y-%m-%d %H:%M:%S", time.localtime()))

def topOnly1():
    write("PID"+","+"VSS"+","+"RSS"+","+"PSS"+","+"USS"+",TIME")
    while True:
        data = getTopLine1()
        write(data[0]+","+data[1]+","+data[2]+","+data[3]+","+data[4]+","+getTime())
        time.sleep(WAIT_TIME)

def getTopLine2():
    tagList=[]
    FIND_NAME2="adb shell top -n 1 -d 1"
    ret = os.popen(CMD.format(FIND_NAME2))
    title=ret.readline()
    title=ret.readline()
    title=ret.readline()
    title=ret.readline()
    title=ret.readline()
    title=ret.readline()
    print(title)
    title2 = re.sub(r"\s{2,}", " ",str(title)).split( )
    tagList.append(title2[1])
    tagList.append(title2[4].replace("G",""))
    tagList.append(title2[5].replace("M",""))
    tagList.append(title2[8])
    return tagList

def topOnly2():
    write("server"+","+"VSS"+","+"RSS"+","+"CPU"+",TIME")
    while True:
        data = getTopLine2()
        write(data[0]+","+data[1]+","+data[2]+","+data[3]+","+getTime())
        time.sleep(WAIT_TIME)
        
def mainRunner():
    while True:
        try:
            if setnum == 0 :
                topOnly0()
            elif setnum == 1 :
                topOnly1()
            elif setnum == 2 :
                topOnly2()
        except IndexError:
            continue
        except UnboundLocalError:
            continue


mainRunner()
    
