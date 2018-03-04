import requests, json
<<<<<<< HEAD

done = False
while done == False:
	with open('./commfifo') as fifo:
	    data = fifo.read()
        jdata = json.loads(data)
        print jdata
        fifo.close()
        r = requests.post('http://localhost:5000/api/v1.0/telemetry', json = jdata)
        print r.text
        if data.find("**&&") != -1:
		done = True
	fifo.close()
=======
launch = requests.get('http://localhost:5000/api/v1.0/prepare_launch')
r = requests.get('http://localhost:5000/api/v1.0/start_launch/'+launch.text)
print r.text
while 1:
    with open('./commfifo') as fifo:
        data = fifo.read()
    if data == "**&&":
        print "End of transmission"
        break
    jdata = json.loads(data)
    print jdata
    fifo.close()
    r = requests.post('http://localhost:5000/api/v1.0/telemetry', json = jdata)
    print r.text
    fifo.close()
r = requests.get('http://localhost:5000/api/v1.0/end_launch/'+launch.text)
print r.text
>>>>>>> 5917ef788261dc3584a2093789f54d6ae1e51656
