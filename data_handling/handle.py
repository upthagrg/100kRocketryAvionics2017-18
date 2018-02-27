import requests, json, time
r = requests.get('http://localhost:5000/api/v1.0/start_launch')
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
    time.sleep(1)
    fifo.close()
r = requests.get('http://localhost:5000/api/v1.0/end_launch')
print r.text