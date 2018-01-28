import requests, json
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
