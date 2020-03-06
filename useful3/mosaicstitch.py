import os
import time
from functools import reduce
from threading import Thread
from PIL import Image
 
 
class MosaicMaker(object):
    # 内部类，执行多线程拼图的任务类
    class __SubTask:
        def __init__(self, n, cur_sub_im, new_im, m, box):
            self.n = n
            self.cur_sub_im = cur_sub_im
            self.new_im = new_im
            self.m = m
            self.box = box
 
        def work(self):
            # print("正在拼第%d张素材" % self.n)
            # 计算key值(灰度值,平均RGB，hash值,三选一)
            cur_sub_key = self.m.cal_key(self.cur_sub_im)
            # 搜索最匹配图片(灰度值,平均RGB，hash值,三选一)
            fit_sub = self.m.find_key(cur_sub_key)
            self.new_im.paste(fit_sub, self.box)
 
    # 内部类，执行多线程读取图库的任务类
    class __ReadTask:
        def __init__(self, n, full_path, fin_w, fin_h, m):
            self.n = n
            self.full_path = full_path
            self.fin_w = fin_w
            self.fin_h = fin_h
            self.m = m
 
        def read(self):
            print("开始读取第%d张图片" % self.n)
            cur = Image.open(self.full_path)
            # 计算key值(灰度值,平均RGB，hash值,三选一)
            key = self.m.cal_key(cur)
            # 将素材缩放到目标大小
            cur = cur.resize((self.fin_w, self.fin_h), Image.ANTIALIAS)
            self.m.get_all_img().update({key: cur})
 
    # 图库目录 目标文件 输出路径 子图尺寸 最小像素单位 拼图模式 默认尺寸
    def __init__(self, db_path, aim_path, out_path, sub_width=64, sub_height=64, min_unit=5, mode="RGB", default_w=1600,
                 default_h=1280):
        self.__db_path = db_path
        self.__aim_path = aim_path
        self.__out_path = out_path
        self.__sub_width = sub_width
        self.__sub_height = sub_height
        self.__min_unit = min_unit
        self.__mode = mode
        self.__default_w = default_w
        self.__default_h = default_h
        self.__all_img = dict()
 
    # 对外提供的接口
    def make(self):
        aim_im = Image.open(self.__aim_path)
        aim_width = aim_im.size[0]
        aim_height = aim_im.size[1]
        print("计算子图尺寸")
        if not self.__divide_sub_im(aim_width, aim_height):
            print("使用默认尺寸")
            aim_im = aim_im.resize((self.__default_w, self.__default_h), Image.ANTIALIAS)
            aim_width = aim_im.size[0]
            aim_height = aim_im.size[1]
        print("读取图库")
        start = time.time()
        self.__read_all_img(self.__db_path, self.__sub_width, self.__sub_height)
        print("耗时：%f秒" % (time.time() - start))
        self.__core(aim_im, aim_width, aim_height)
 
    def __core(self, aim_im, width, height):
        new_im = Image.new("RGB", (width, height))
        # 每行每列的图片数
        w = width // self.__sub_width
        print("源文件尺寸为：（w:%d  h:%d）" % (width, height))
        print("子图的尺寸为：（w:%d  h:%d）" % (self.__sub_width, self.__sub_height))
        print("w:%d" % w)
        print("开始拼图,请稍等...")
        start = time.time()
        n = 1
        thread_list = list()
        for i in range(w):
            task_list = list()
            for j in range(w):
                # 多线程版
                left = i * self.__sub_width
                up = j * self.__sub_height
                right = (i + 1) * self.__sub_width
                down = (j + 1) * self.__sub_height
                box = (left, up, right, down)
                cur_sub_im = aim_im.crop(box)
                t = self.__SubTask(n, cur_sub_im, new_im, self, box)
                task_list.append(t)
                n += 1
            thread = Thread(target=self.__sub_mission, args=(task_list,))
            thread_list.append(thread)
        for t in thread_list:
            t.start()
        for t in thread_list:
            t.join()
        print("拼图完成,共耗时%f秒" % (time.time() - start))
        # 将原图与拼图合并，提升观感
        new_im = Image.blend(new_im, aim_im, 0.35)
        new_im.show()
        new_im.save(self.__out_path)
 
    # 拼图库线程执行的具体函数
    @staticmethod
    def __sub_mission(missions):
        for task in missions:
            task.work()
 
    # 计算子图大小
    def __divide_sub_im(self, width, height):
        flag = True
        g = self.__gcd(width, height)
        if g < 20:
            flag = False
            width = self.__default_w
            height = self.__default_h
            g = 320
 
        if g == width:
            g = 320
        self.__sub_width = self.__min_unit * (width // g)
        self.__sub_height = self.__min_unit * (height // g)
        return flag
 
    # 读取全部图片,按(灰度值,平均RGB，hash值)保存 fin_w,fin_h素材最终尺寸
    def __read_all_img(self, db_path, fin_w, fin_h):
        files_name = os.listdir(db_path)
        n = 1
        # 开启5个线程加载图片
        ts = list()
        for i in range(5):
            ts.append(list())
        for file_name in files_name:
            full_path = db_path + "\\" + file_name
            if os.path.isfile(full_path):
                read_task = self.__ReadTask(n, full_path, fin_w, fin_h, self)
                ts[n % 5].append(read_task)
                n += 1
        tmp = list()
        for i in ts:
            t = Thread(target=self.__read_img, args=(i,))
            t.start()
            tmp.append(t)
        for t in tmp:
            t.join()
 
    # 读取图库线程执行的具体函数
    @staticmethod
    def __read_img(tasks):
        for task in tasks:
            task.read()
 
    # 计算key值
    def cal_key(self, im):
        if self.__mode == "RGB":
            return self.__cal_avg_rgb(im)
        elif self.__mode == "gray":
            return self.__cal_gray(im)
        elif self.__mode == "hash":
            return self.__cal_hash(im)
        else:
            return ""
 
    # 获取key值
    def find_key(self, im):
        if self.__mode == "RGB":
            return self.__find_by_rgb(im)
        elif self.__mode == "gray":
            return self.__find_by_gray(im)
        elif self.__mode == "hash":
            return self.__find_by_hash(im)
        else:
            return ""
 
    # 计算灰度值
    @staticmethod
    def __cal_gray(im):
        if im.mode != "L":
            im = im.convert("L")
        return reduce(lambda x, y: x + y, im.getdata()) // (im.size[0] * im.size[1])
 
    # 计算平均rgb值
    @staticmethod
    def __cal_avg_rgb(im):
        if im.mode != "RGB":
            im = im.convert("RGB")
        pix = im.load()
        avg_r, avg_g, avg_b = 0, 0, 0
        n = 1
        for i in range(im.size[0]):
            for j in range(im.size[1]):
                r, g, b = pix[i, j]
                avg_r += r
                avg_g += g
                avg_b += b
                n += 1
        avg_r /= n
        avg_g /= n
        avg_b /= n
        return str(avg_r) + "-" + str(avg_g) + "-" + str(avg_b)
 
    # 计算hash
    def __cal_hash(self, im):
        im = im.resize((8, 8), Image.ANTIALIAS)
        im = im.convert("L")
        avg_gray = self.__cal_gray(im)
        k = ""
        _0 = "0"
        _1 = "1"
        for i in im.getdata():
            if i < avg_gray:
                k += _0
            else:
                k += _1
        return k
 
    # 辗转相除法求最大公约数
    @staticmethod
    def __gcd(a, b):
        while a % b:
            a, b = b, a % b
        return b
 
    # 获取最佳素材(按灰度)
    def __find_by_gray(self, gray):
        m = 255
        k = 0
        for key in self.__all_img.keys():
            cur_dif = abs(key - gray)
            if cur_dif < m:
                k = key
                m = cur_dif
        return self.__all_img[k]
 
    # 获取最佳素材(按pHash)
    def __find_by_hash(self, sub_hash):
        m = 65
        k = 0
        for key in self.__all_img.keys():
            cur_dif = self.__dif_num(sub_hash, key)
            if cur_dif < m:
                k = key
                m = cur_dif
        return self.__all_img[k]
 
    @staticmethod
    def __dif_num(hash1, hash2):
        n = 0
        for i in range(64):
            if hash1[i] != hash2[i]:
                n += 1
        return n
 
    # # 获取最佳素材(按平均rgb)
    def __find_by_rgb(self, sub_rgb):
        sub_r, sub_g, sub_b = sub_rgb.split("-")
        m = 255
        k = ""
        for key in self.__all_img.keys():
            src_r, src_g, src_b = key.split("-")
            cur_dif = abs(float(sub_r) - float(src_r)) + abs(float(sub_g) - float(src_g)) + abs(
                float(sub_b) - float(src_b))
            if cur_dif < m:
                m = cur_dif
                k = key
        return self.__all_img[k]
 
    def get_all_img(self):
        return self.__all_img
 
 
if __name__ == '__main__':
    m = MosaicMaker("20200305", "2020010609400065-scaled.jpeg",
                    "out.jpeg")
    m.make()
    pass
