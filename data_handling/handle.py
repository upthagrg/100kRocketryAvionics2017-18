import requests, json
launch = requests.get('http://localhost:5000/api/v1.0/prepare_launch')
r = requests.get('http://localhost:5000/api/v1.0/start_launch/'+launch.text)
print r.text
flag1 = True;
flag2 = True;
while 1:
    if flag1 == True:
      with open('./commfifo1') as fifo1: # booster data
          data1 = fifo1.read()
    if flag2 == True
      with open('./commfifo2') as fifo2: # sustainer data
          data2 = fifo2.read()
    if data1 == "**&&":
        print "End of booster transmission"
	flag1 = False
        #break
    if data2 == "**&&":
        print "End of sustainer transmission"
	flag2 = False
        #break
    if flag1 == True:
    	jdata = json.loads(data1)
    	print jdata
    	r = requests.post('http://localhost:5000/api/v1.0/telemetry', json = jdata)
    	print r.text
    	fifo1.close()
    if flag2 == True:
    	jdata = json.loads(data2)
    	print jdata
    	r = requests.post('http://localhost:5000/api/v1.0/telemetry', json = jdata)
    	print r.text
    	fifo2.close()
    if (flag1 == False) && (flag2 == False):
	break
r = requests.get('http://localhost:5000/api/v1.0/end_launch/'+launch.text)
print r.text
