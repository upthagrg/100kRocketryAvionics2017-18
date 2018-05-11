print "start"
import requests, json
print "making request"
launch = requests.get('http://172.17.0.4:5000/api/v1.0/prepare_launch')
print "request finished"
print launch.text
print "end of print"
r = requests.get('http://172.17.0.4:5000/api/v1.0/start_launch/'+launch.text)
print "continueing"
print r.text
flag1 = True;
flag2 = True;
print "starting";
while 1:
    if flag1 == True:
      print "opening commfifo1";
      with open('./commfifo1') as fifo1: # booster data
	  print "python reading commfifo1";
          data1 = fifo1.read()
	  print "Python got from commfifo1:";
	  print(data1)
    if flag2 == True:
      print "opening commfifo2";
      with open('./commfifo2') as fifo2: # sustainer data
	  print "python reading commfifo2";
          data2 = fifo2.read()
	  print "Python got from commfifo2:";
	  print(data2)
    if data1 == "**&&" or data1 == "**":
        print "PYTHON End of booster transmission"
	flag1 = False
        #break
    if data2 == "**&&" or data2 == "**":
        print "PYTHON End of sustainer transmission"
	flag2 = False
        #break
    if flag1 == True:
    	jdata = json.loads(data1)
    	print jdata
    	r = requests.post('http://172.17.0.4:5000/api/v1.0/telemetry', json = jdata)
    	print r.text
    	fifo1.close()
    if flag2 == True:
    	jdata = json.loads(data2)
    	print jdata
    	r = requests.post('http://172.17.0.4:5000/api/v1.0/telemetry', json = jdata)
    	print r.text
    	fifo2.close()
    if ((flag1 == False) and (flag2 == False)):
	break
r = requests.get('http://172.17.0.4:5000/api/v1.0/end_launch/'+launch.text)
print r.text
