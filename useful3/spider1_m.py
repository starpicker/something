"""导入模块"""
import requests  # 导入requests库
import re  # 导入正则表达式库
import os  # 导入操作系统库
import time  # 导入时间库
print("爬虫程序正在执行中，因设置有延时，故页面显示有些迟缓……")
def dir_save():
    dir_name = re.findall('<h1 class="post-title h3">(.*?)</h1>', html)[-1]  # 正则表达式创建目录名字
    if not os.path.exists(dir_name):  # 判断文件夹是否存在，如果不存在：
        os.mkdir(dir_name)  # 创建一个文件夹
    with open(dir_name + '/' + file_name, 'wb') as f:  # 用wb模式打开创建文件，w写模式
        f.write(response.content)  # 写入二进制文件内容
user = {
    'user-agent': 'Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/69.0.3497.100 Safari/537.36'
}
response = requests.get('https://www.vmgirls.com/sitemap.shtml',headers=user)  # 用requests库的get函数访问总网页，用headers进行伪装，获得所有文章网址
html = response.text  # 用文本显示访问网页得到的内容
urls_wz = re.findall('https://www.vmgirls.com/\d*.html', html)  # 用正则表达式获得文章的所有网址
#print(urls_wz)  # 打印显示所有网址
count = 0
for url_wz in urls_wz:
    count += 1
    if count < 20:
        continue
#    time.sleep(30)  # 设定30秒延时
    time.sleep(10)
    print('正在爬取：' + str(url_wz) + "的图片")  # 查看正在执行的网址，以判断是否下载完整
    response = requests.get(url_wz, headers=user)  # 用requests库的get函数访问网页，用headers进行伪装
    #print(response.text)    #打印网页
    html = response.text  # 用文本显示访问网页得到的内容
    urls_tp = re.findall('<a href="(.*?)" alt=".*?" title=".*?">', html)  # 正则表达式获得图片网址
    #print(urls_tp)  # 打印显示文章图片网址
    if len(urls_tp):
        for url_tp in urls_tp:  # 循环获取每一个图片网址
            # 图片的名字
            file_name = url_tp.split('/')[-1]  # 取每个url_tp最后的部分
            time.sleep(10)  # 设定10秒延时
            response = requests.get(url_tp, headers=user)  # 用requeste库的get函数访问图片网址，用headers进行伪装
            print("正在保存图片中……")
            dir_save()
        print("保存第一页图片完毕！！！")
        for i in range(2,100):  #用range（）函数分别输出2-99
            url_wz = "".join(url_wz)
            url_dfy = re.findall('(.*?).html',url_wz)
            url_dfy = "".join(url_dfy)
            url_fy = url_dfy + '/page-' + str(i) + '.html'
            #print(url_fy)
            time.sleep(30)  # 设定30秒延时
            response = requests.get(url_fy,headers = user)
            html = response.text
            urls_fy_pic = re.findall('<a href="(.*?)" alt=".*?" title=".*?">', html)
            #print(urls_fy_pic)    #查看图片网址
            if len(urls_fy_pic):
                for url in urls_fy_pic:  # 循环获取每一个图片网址
                    file_name = url.split('/')[-1]  # 取每个url最后的部分
                    time.sleep(5)  # 设定5秒延时
                    response = requests.get(url, headers=user)  # 用requeste库的get函数访问图片网址，用headers进行伪装
                    print("正在保存第%s页的图片……"%str(i))
                    dir_save()
                print("第%s页保存成功"%str(i))
            else:
                print("第%s页面无图片，即将爬取下一篇文章"%str(i))
                break
    else:
        print("原有正则表达式保存不成功的网址是：" + str(url_wz))

