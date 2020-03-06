'''
 
爬取妹子图，可自定义分类下载图片
 
'''
 
import urllib.request
from lxml import etree
import time,random,os,ssl
 
class Spider_Meizi():
    #初始化
    def __init__(self,file_path):
        #定义全局变量
        self.file_path = r"C:\Users\sai\PycharmProjects\untitled\Learn\爬虫\实战-meizitu"
        self.agent_list = [
            'Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/536.11 (KHTML, like Gecko) Chrome/20.0.1132.11 TaoBrowser/2.0 Safari/536.11',
            'Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.1 (KHTML, like Gecko) Chrome/21.0.1180.71 Safari/537.1 LBBROWSER',
            'Mozilla/5.0 (compatible; MSIE 9.0; Windows NT 6.1; WOW64; Trident/5.0; SLCC2; .NET CLR 2.0.50727; .NET CLR 3.5.30729; .NET CLR 3.0.30729; Media Center PC 6.0; .NET4.0C; .NET4.0E; LBBROWSER)',
            'Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; QQDownload 732; .NET4.0C; .NET4.0E; LBBROWSER)',
            'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/76.0.3809.100 Safari/537.36',
            'Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Maxthon/4.4.3.4000 Chrome/30.0.1599.101 Safari/537.36',
            'Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Maxthon/4.4.3.4000 Chrome/30.0.1599.101 Safari/537.36'
        ]
        self.headers = {"User-Agent":"Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_3) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/79.0.3945.130 Safari/537.36",
          "Referer":"https://www.mzitu.com"}
        self.realm_url = r"https://www.mzitu.com/"
        self.img_type = "mm"
 
 
    #随机取User-agent并更新headers
    def ReturnUA(self):
        self.headers["User-Agent"] = random.choice(self.agent_list)
 
    #解析url地址 url:分类的首页
    def AnalysisUrl(self,url):
        self.ReturnUA()
        #调试用
        print("正在解析："+url)
 
        res = urllib.request.Request(url,headers=self.headers)
        rep_data = urllib.request.urlopen(res).read().decode("utf-8")
        #当前页的内容
        html_data = etree.HTML(rep_data)
        #调试用
        #print(url+" 解析成功")
        return html_data
 
    def AnalysisPage(self,html_data):
        #递归取当前页的每一个小类里的内容 img_url_list 首页里每个小块的url地址
        img_url_list = html_data.xpath('//div[@class="main"]//li/a/@href')
        img_name_list = html_data.xpath('//div[@class="main"]//li/a/img/@alt')
        #temp_url 每个小块的url（https://www.mzitu.com/223466）
        for temp_url,temp_name in zip(img_url_list,img_name_list):
            #调试用
            #print(temp_url,temp_name)
            img_path = os.path.join(self.file_path,temp_name)
            temp_html_data = self.AnalysisUrl(temp_url)
            # 取当前网页的页数
            img_pages_list = temp_html_data.xpath('//div[@class="pagenavi"]//a/span/text()')
            #调试用
            #print(img_pages_list)
            try:
                img_pages_list.remove("下一页&#187;")
            except ValueError:
                print("删除：'下一页&#187;'失败")
            try:
                img_pages_list.remove("&#171;上一组")
            except ValueError:
                print("删除：'&#171;上一组'失败")
            img_pages_list_num = list(map(int,img_pages_list))
            max_ima_page = max(img_pages_list_num)
            print("当前图片共有："+str(max_ima_page)+"页！")
            i = 1
            for i in range(i,max_ima_page+1):
                #new_img_url(https://www.mzitu.com/223466/2)
                new_img_url = temp_url + "/" + str(i)
                img_html_data = self.AnalysisUrl(new_img_url)
                #img_url 每页图片的url地址（[img]https://i3.mmzztt.com/2020/02/23a02.jpg[/img]）
                img_url = img_html_data.xpath('//div[@class="main-image"]//a/img/@src')[0]
 
                img_req = urllib.request.Request(img_url, headers=self.headers)
                #调试用
                #print(img_url+"解析成功！")
                #print(img_req)
                img_data = urllib.request.urlopen(img_req).read()
                print("图片数据读取完成！准备写文件......")
                img_name = str(i)+".jpg"
                self.SaveImage(img_data,img_name,img_path,temp_name)
                # 根据需求调整下载速度，太快容易被封IP
                time.sleep(1)
 
 
    #获取图片地址，保存至本地
    def SaveImage(self,img_data,img_name,img_path,temp_name):
        #判断目录是否存在，如果不存在创建目录
        if os.path.exists(self.file_path) == False:
            os.mkdir(self.file_path)
        if os.path.exists(img_path) == False:
            os.mkdir(img_path)
            print("成功创建目录："+img_path)
        os.chdir(img_path)
        print("当前目录："+ img_path+"。开始保存图片......")
        try:
            #print("开始写入文件："+temp_name+"："+img_name)
            with open(img_name, "wb") as f:
                f.write(img_data)
        except:
            print("写入文件错误！")
 
 
    def StartProgram(self):
        #让用户输入下载的类型，输入错误超过3次设置为默认类型
        input_txt = input("请输入要下载的图片类型!\n 可选类型有：xinggan、japan、taiwan、mm、zipai、jiepai:")
        for i in range(1, 4):
            if input_txt in ("xinggan","japan","taiwan","mm","zipai","jiepai"):
                self.img_type = input_txt
                break
            else:
                if i == 3:
                    print("连续输入错误，默认mm类型！")
                else:
                    input_txt = input("输入有误！请重新输入(xinggan、japan、taiwan、mm、zipai、jiepai)：")
        #让用户输入下载页数，并进行逻辑判断
        print("所选类型为："+self.img_type)
        #根据所选 改变文件路径
        self.file_path = os.path.join(self.file_path,self.img_type)
        first_page = int(input("请输入起始下载页："))
        if first_page<=0:
            print("输入有误。默认为第1页！")
            first_page = 1
        end_page = int(input("请输入结束下载页："))
        if end_page <= 0:
            print("输入有误。默认为第2页！")
            end_page = 2
        if first_page > end_page:
            print("起始页大于结束页，对调处理！")
            temp_page = first_page
            first_page = end_page
            end_page = temp_page
        print("选择页数范围："+str(first_page)+"-"+str(end_page))
        i = first_page
        while i <= end_page:
            html_url = self.realm_url +self.img_type+"/page/" + str(i) + "/"
            ori_html_data = self.AnalysisUrl(html_url)
            self.AnalysisPage(ori_html_data)
            i += 1
 
 
if __name__ == "__main__":
    ori_file_path = r"meizitu"
    Mzt = Spider_Meizi(file_path=ori_file_path)
    Mzt.StartProgram()
