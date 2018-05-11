import time, datetime
with open('test_data2', 'w') as filehandle:
    while 1:
        time.sleep(1)
        filehandle.write(str(datetime.datetime.now())+"\n")