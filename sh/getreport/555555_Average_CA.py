#!/usr/bin/python

# coding = utf-8
import sys
from PIL import Image
import os,time,shutil

'''
verison: 1.0.1.20170216
Author: ArcSoft
change log: only static the  chromatic aberration percent between different range.
'''

def get_RGB(photoPath, Module):
    """
    get the brightness of all of the photo in the photoPath, and return the dictory.
    """
    dic_photo={}

    for photo in os.listdir(photoPath):
        if photo[-3:] not in ["jpg","JPG"]:
            continue

        abs_photo_path = os.path.join(photoPath,photo)
        im_temp = Image.open(abs_photo_path)

        if "_1.jpg"  in photo and Module =="W+T":
            box = (im.size[0]/4,im.size[1]*1/4,im.size[0]*3/4,im.size[1]*3/4)
            #box = (im.size[0]*8/25,im.size[1]*8/25,im.size[0]*17/25,im.size[1]*17/25)
            im_crop = im.crop(box)
            im_crop.save("temp.jpg")
            im_temp = Image.open("temp.jpg")

        w,h=im_temp.size
        #im = im_temp.resize((w/2,h/2)) #for 12MP photo,it need 25s, so resize to 50%
        im = im_temp.resize((w/2,h/2))
        R,G,B,ns=0,0,0,0

        for w0 in range(0,w/2):
            for  h0 in range(0,h/2):
                r,g,b = im.getpixel((w0,h0))
                R += r
                G += g
                B += b
                ns = ns+1

        PhotoName = photo.split("_")[1]

        print photo,R/ns,G/ns,B/ns

        dic_photo.setdefault(PhotoName,[]).append(photo)
        dic_photo.setdefault(PhotoName, []).append((float(R)/G,float(B)/G))

    return dic_photo

def Compare(x,y): # compare two photo ,and return tuple which max is first
    if x>y:
        return x,y
    else:
        return y,x

def get_percent_CA(dic_photo,compare_channel):
    level1, level2 = 0,0   #level 1 means less than [0,5%],level 2 means more than 5%
    max_offset , offset_total=0,0
    max_offset_photo_name = ""

    if compare_channel =="R/G":
        index = 0
    elif compare_channel =="B/G":
        index =1

    for value in dic_photo.keys():
        offset = 0
        if len(dic_photo[value])<4:
            continue

        compare_result = Compare(dic_photo[value][1][index],dic_photo[value][3][index])

        offset = float(compare_result[0]-compare_result[1])/compare_result[0]

        #print value, "%.2f%%"%(offset*100)

        if max_offset < offset:
            max_offset = offset
            max_offset_photo_name=value

        offset_total = offset_total+ offset

        if offset <0.05 or offset == 0.05:
            level1 += 1
        else:
            level2 += 1
    total = level1 + level2

    saveout = sys.stdout
    fcon = open('conclusion/whitebalance.txt', 'a')
    sys.stdout = fcon
    print "\n***********************************Conclusion******************************\n"

    print "The CA offset for %s  between (0,5%%) is %.2f%%\n"%(compare_channel, float(level1)/total*100)

    print "Max offset is %.2f%%, and average offset is %.2f%%"%(max_offset*100,offset_total*100/total)

    print "\n***********************************Conclusion******************************\n"

    sys.stdout = saveout
    fcon.close()

if __name__=="__main__":
    time_start = time.clock()

    Module_class={1:"RGB+RGB", 2:"RGB+MONO",3:"W+T"}

    #index = int(argv[1])
    #photo_path = argv[2]
    
    index = 1
    photo_path = r"jpg" 

    print "module is %s, photo_path is %s" %(Module_class[index], photo_path)

    resDir = r"conclusion"
    os.system("mkdir -p " + resDir)

    RGB_dic = get_RGB(photo_path,Module_class[index])

    get_percent_CA(RGB_dic,"R/G")

    get_percent_CA(RGB_dic,"B/G")

    print "Cost total time is %f s"%(time.clock()-time_start)
