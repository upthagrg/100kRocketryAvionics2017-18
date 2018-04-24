#interface to Teledongle over USB. This uses the pyUSB module and its utility library.
import usb.core
import usb.util
import sys

#defines
altus = 0xfffe #altus metrum vendor id
teledongle = 0x000c #Teledongle devic id
config = 1 #configuration
intf = 0 #interface
alternate = 0 #alternate 
ep = 1 #endpoint

#find teledongle
device = usb.core.find(idVendor=altus, idProduct=teledongle)

#sanity check
if device is None:
	raise ValueError("Teledongle not found")
else:
	print "Teledongle found"

#remove kernel devices 
#use interface 0, reason: magic
if device.is_kernel_driver_active(0):
	print("Detaching kernel driver")
	device.detach_kernel_driver(0)

#set configuration to first found 
#device.set_configuration() #I have yet to have this work with good results

#get endpoint
configuration = device.get_active_configuration()
interface = configuration[(intf, alternate)]

endpoint = interface[ep]

#sanity check
assert endpoint is not None

print "Telemega configured successfully"

#Teledongle params
freq = 'c F 434550'
data_rate = 'c T 0'
listen = 'm 20'

#write params
device.write(endpoint, freq, 100)
device.write(endpoint, data_rate, 100)
device.write(endpoint, listen, 100)

#data buffer
buff = ''

#start indefintie loop for listening to Teledongle
while(True):
	token = device.read(endpoint, 1, 100)
	buff += token
	print(token)
	print(buff)
