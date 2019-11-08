# CPTest7.py, demonstrate Python binding
# Demonstrate processing of a result set using dynamic SQL
import codecs, sys
import intersys.pythonbind3
id=1;
user="_system";
password="sys";
host = "196.1.10.14";
port = "1972";
query = "A";
name=""
cardid=""
phone=""


def isnumeric(s):
  return all(c in "0123456789" for c in s)

def query(sql, database):
    found = False
    print (sql)
    cq = intersys.pythonbind3.query(database)
    sqlcode=0;
    cq.prepare(sql);
##    cq.set_par(1,query);
    cq.execute();
    while 1:
        cols = cq.fetch([None])
        if len(cols) == 0:
            break
        found = True
        # dump the columns in each row
        #@line = join ":", @cols;
        print (cols)
        
    return found

args = sys.argv[1:]
if len(args) == 0:
    exit(0)
    
index = 0
while 1:
    if index >= len(args): break
    value = args[index]
    if isnumeric(value):
        if len(value) == 11:
            phone = value
        elif len(value) >= 18:
            cardid = value
    else:
        name = value
    index += 1

try:
    
    
    # Connect to specified machine, in the SAMPLES namespace
    url = host+"["+port+"]:DHC-APP"  
    conn = intersys.pythonbind3.connection( )
    conn.connect_now(url, user, password, None)
    database = intersys.pythonbind3.database( conn)
    
    # Create the SQL statement */
    sql = '''SELECT PAPMI_No AS 登记号, PAPMI_RowID1, * FROM PA_PatMas WHERE PAPMI_Name  LIKE "%'''+ \
          name \
          + '''%" AND (papmi_dvanumber="''' + cardid +  '''" OR PAPMI_ID="''' + cardid + '''")''';
    
    ret = query(sql, database)
    if ret == False:
        sql = '''SELECT PAPMI_No AS 登记号, * FROM PA_PatMas WHERE PAPMI_RowID1=(SELECT PAPER_RowId FROM PA_Person WHERE PAPER_TelH="''' + \
              phone + '''" OR PAPER_ID = "''' + cardid + '''")''';
        ret = query(sql, database)
        if ret == False:
            sql = '''SELECT PAPMI_No AS 登记号, PAPMI_RowID1, * FROM PA_PatMas WHERE PAPMI_Name  LIKE "%'''+ \
                  name \
                  + '''%" or (papmi_dvanumber="''' + cardid +  '''" OR PAPMI_ID="''' + cardid + '''")''';
            ret = query(sql, database)
            
except intersys.pythonbind3.cache_exception ( err ):
    print ("InterSystems Cache' exception")
    print (sys.exc_type)
    print (sys.exc_value)
    print (sys.exc_traceback)
    print (str(err))

print ("query end")
input()
