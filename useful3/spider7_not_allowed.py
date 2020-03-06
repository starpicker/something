# coding = utf8
# https://www.meitulu.com/t/xinggan/
# created by XSX
 
 
import requests
from lxml import etree
import os
import time
#from GetRandomheader import Randomheader
import random
 
def Randomheader():
	ua = [  "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/60.0.3112.113 Safari/537.36", #chrome
			"Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1; Trident/4.0; SE 2.X MetaSr 1.0; SE 2.X MetaSr 1.0; .NET CLR 2.0.50727; SE 2.X MetaSr 1.0)", #搜狗
			"Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1; 360SE)", #360
			"Mozilla/5.0 (Windows NT 6.1; WOW64; rv:34.0) Gecko/20100101 Firefox/34.0" #火狐
		 ]
	headers ={
				"User-Agent":random.choice(ua)
			 }
	return headers

 
def GetPhotopage(url, headers):
    r = requests.get(url, headers=headers)
    r.encoding = r.apparent_encoding
    html = etree.HTML(r.text)
    Photospagelists = html.xpath('//div[@class="boxs"]/ul/li/a/@href')
    print(Photospagelists)
    return Photospagelists
 
 
def GetPagelinks(Photospagelists, headers):
    # get Double lists
    PageLinks1 = []
    for Photospagelist in Photospagelists:
        r1 = requests.get(Photospagelist, headers=headers)
        html1 = etree.HTML(r1.text)
        PageLinks = html1.xpath('//div[@id="pages"]/a/@href')
        PageLinks1.append(PageLinks)
    print(PageLinks1)
    return PageLinks1
 
 
def GetDownlaodJPGlist(PageLinks1):
    listCatch = []
    JPGlinks = []
    for Jpglists in PageLinks1:
        listCatch.append(Jpglists)
        for listCatch1 in listCatch:
            for listCatch2 in listCatch1:
                datacacth1 = "https://www.meitulu.com" + str(listCatch2)
                JPGlinks.append(datacacth1)
    print(JPGlinks)
    return JPGlinks
 
 
def DownlaodJPG(JPGlinks, headers):
    if not os.path.exists("./JPG"):
        os.mkdir("./JPG")
    for JPGlink in JPGlinks:
        time.sleep(0.05)
        r3 = requests.get(JPGlink, headers=headers)
        html2 = etree.HTML(r3.text)
        jpglistss = html2.xpath('//div[@class="content"]/center/img/@src')
        for jpglists in jpglistss:
            name = str(jpglists).split('/')[-1]
            filename = str(jpglists).split('/')[-2]
            if not os.path.exists("./JPG/" + filename):
                os.mkdir("./JPG/" + filename)
            time.sleep(0.1)
            r4 = requests.get(jpglists, headers=headers)
            with open('./JPG/' + filename + '/' + name, 'wb')as f:
                f.write(r4.content)
    print(filename)
    print("Process Success!")
 
 
if __name__ == '__main__':
    # url1 = "https://www.meitulu.com/t/xinggan/"
    url1 = "https://www.meitulu.com/t/meixiong/"
    DownlaodJPG(GetDownlaodJPGlist(GetPagelinks(GetPhotopage(url1, Randomheader()), Randomheader())), Randomheader())
