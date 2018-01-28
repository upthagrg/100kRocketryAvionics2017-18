import requests, json
import sys
done = False
while done == False:
	with open('./commfifo') as fifo:
	    data = fifo.read()
        if data == "**&&":
            print "End of transmission"
            sys.exit()
        jdata = json.loads(data)
        print jdata
        fifo.close()
        r = requests.post('http://localhost:5000/api/v1.0/telemetry', json = jdata)
        print r.text
        if data.find("**&&") != -1:
		done = True
	fifo.close()
