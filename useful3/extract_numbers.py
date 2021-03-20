str= "了很怀念：333.2，地塞米松：33.875，床位费：17， 糖化 ：38"
sum = 0
number = ""
is_num ="0123456789" #定义是否是数字
is_has_float = False #定义是否是小数点
isstart = False #定义是否是数字开始的标记变量
for a in str: #将数字循环遍历
 if a in is_num:      #判断取出来的数字是否是数字
  if isstart==True:
   number = number+a
  else:
   number = number+a
   isstart=True
  continue
 if a=='.':       #判断取出来的数字是否是小数点
  if isstart==True:    #判断前一个是否是数字
   if is_has_float == True:
    pass
   else:
    is_has_float = True
    number = number+a
    continue
 if isstart == True: #当一个数字遍历完了，就要求和，现在取出的数字格式的字符串，不是整形，所以6+7 = 67
  print(number.strip('.')) #strip去掉最后一个小数点
  sum = sum+float(number.strip('.'))
  #前一个数字取完了之后，初始化，
  number = ""
  isstart = False
  is_has_float = False
if isstart == True: #判断结尾是否是数字
 print(number.strip("."))
 sum = sum+float(number.strip("."))
print(sum)
