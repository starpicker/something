import requests
from lxml import etree
from urllib.request import urlretrieve
from urllib.parse import urlencode
import re
import os
import time
import random
ua = [  "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/60.0.3112.113 Safari/537.36", #chrome
        "Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1; Trident/4.0; SE 2.X MetaSr 1.0; SE 2.X MetaSr 1.0; .NET CLR 2.0.50727; SE 2.X MetaSr 1.0)", #搜狗
		"Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1; 360SE)", #360
		"Mozilla/5.0 (Windows NT 6.1; WOW64; rv:34.0) Gecko/20100101 Firefox/34.0" #火狐
	 ]
headers ={
			"User-Agent":random.choice(ua)
	     }
print(random.choice(ua))
for i in range(4,8):  #可以自己定义爬取页数  这个文档就爬取1页
	url = "http://www.umei.cc/meinvtupian/meinvxiezhen/"+str(i)+".htm"
	res = requests.get(url,headers=headers)
	data = res.content.decode('utf-8')
	treeData = etree.HTML(data)
	title = treeData.xpath('//title/text()')
	print(title)
	urls = treeData.xpath('//*[@class="TypeList"]/ul/li/a/@href') #起始地址url
	titles = treeData.xpath('//*[@class="TypeList"]/ul/li/a/div/text()') #所有的url起始地址名称
	print(urls)
	print(titles)
	for j in range(0,len(urls)):
		k=titles[j]
		file = 'photos/'+k #D盘创建一个photos 文件夹 保存照片
		# 判断文件是否存在，不存在则创建
		#获取页数
		res1 = requests.get(urls[j],headers=headers)
		data1 = res1.content.decode('utf-8')
		part1 = '<li><a>共(.*?)页'
		url_num =re.compile(part1,re.S).findall(data1)
		print(url_num[0])
		if not os.path.exists(file):
			os.makedirs(file)
			print('--创建成功--')

		for n in range(0,eval(url_num[0])):
			if n==0 :
				urls3 = urls[j]
			else:
				urls3 = urls[j][:-4]+'_'+str(n+1)+'.htm'
				# print(urls3)
			print(urls3)
			try:
				res2 = requests.get(urls3,headers=headers)
				data2 = res2.content.decode('utf-8')
				# print(data1)
				part2 = '<img alt.*?src="(.*?)" />'
				photo_url = re.compile(part2,re.S).findall(data2)
				print(photo_url)
				path = 'photos/'+k+'/'+str(n+1)+'.jpg'
				urlretrieve(photo_url[0],path)
				print("下载完成，程序挂起0.5s") #防止爬取速度过快
				time.sleep(5)
			except Exception as err:
				print(err)
