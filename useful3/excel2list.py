import xlrd

def excel2list(file, sheetname='', startrow=0):
    wb = xlrd.open_workbook(file)
    st = (lambda sn: wb.sheets()[0] if sn=='' else  wb.sheet_by_name(sn))(sheetname)
        
    rows = st.nrows
    cols = st.ncols
    
    list = []
    for row in range(startrow, rows):
        row = st.row_values(row)
        l = []
        for i in range(cols):
            l.append(row[i])
        list.append(l)
        
    return list

if __name__ == '__main__':
    l = excel2list(r"C:\Users\dell\Desktop\1.xlsx", "Sheet1", 6)
    for i in l:
        print(i)

