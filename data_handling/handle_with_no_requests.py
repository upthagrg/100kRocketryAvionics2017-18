done = False
while done == False:
	with open('./commfifo') as fifo:
		data = fifo.read()
	print data
	if data.find("**&&") != -1:
		done = True
	fifo.close()
