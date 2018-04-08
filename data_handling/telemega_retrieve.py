import usb.core
import usb.util
import sys

#deines
altus = 0xfffe
teledongle = 0x000c
config = 1
intf = 0
alternate = 0
ep = 1

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
#device.set_configuration()

#get endpoint
configuration = device.get_active_configuration()
interface = configuration[(intf, alternate)]

endpoint = interface[ep]

#sanity check
assert endpoint is not None

print "Telemega configured successfully"

#msg = 'test'
#assert len(device.write(1, msg, 100)) == len(msg)
#ret = device.read(0x81, len(msg), 100)
#sret = ''.join([chr(x) for x in ret])
#assert sret == msg
