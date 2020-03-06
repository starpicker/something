import openpyxl
wb = openpyxl.load_workbook('111.xlsx')
sheet = wb['Sheet00']

for rowOfCellObjects in sheet['A1':'D49']:
    for cellObj in rowOfCellObjects:
        if(cellObj.value):
            print('{0:{1}<9}\t'.format(cellObj.value, chr(12288)), end = '')
##        print(cellObj.value, end = ' ')
    print('\n')
