#***********************************************************
# Title: getmaps.py
# Author: Glenn Upthagrove
# Date: 01/27/2018
# Descrition: Creates a .bmp image of a map anywhere in the
# world using a latitude and longitude taken from the 
# command line as cener fro the map
#***********************************************************

import requests
import subprocess
import sys
from PIL import Image

#***********************************************************
# Title: make_url
# Descrition: Creates a URL for use with the Google Static
# Maps API.
#***********************************************************
def make_url(lat, lon):
	url = "http://maps.google.com/maps/api/staticmap?maptype=satellite&center="
	url = url + lat
	url = url + ","
	url = url + lon
<<<<<<< HEAD
	url = url + "&zoom=14&size=512x512&format=jpg"
	return url

#define names and paths for images
jpgname = '/home/glenn/100k6/100kRocketryAvionics2017-18/3d_trace/resources/textures/map.jpg'
bmpname = '/home/glenn/100k6/100kRocketryAvionics2017-18/3d_trace/resources/textures/map.bmp'
=======
	url = url + "&zoom=14&size=512x512"
	return url

#define names and paths for images
pngname = '../resources/textures/map.png'
bmpname = '../resources/textures/map.bmp'
>>>>>>> 94938cdc9e6390c12af0013c86aea31433c12796
#define default latitude and longitude 
deflat = '44.565028'
deflon = '-123.276136'
#open a session
mapsession = requests.Session()
#create the url
if(len(sys.argv) != 3): #check for latitude and longitude arguments
	url = make_url(deflat, deflon)
else:
	url = make_url(sys.argv[1], sys.argv[2])
#get the map as a binary
mapbin = mapsession.get(url)
#open a .png file
<<<<<<< HEAD
mapfile = open(jpgname ,'wb')
#write the binary map to the .jpg
=======
mapfile = open(pngname ,'wb')
#write the binary map to the .png
>>>>>>> 94938cdc9e6390c12af0013c86aea31433c12796
mapfile.write(mapbin.content)
#close the .png file
mapfile.close()
#open the .png as an Image
<<<<<<< HEAD
pngmap = Image.open(jpgname)
#save the Image as a .bmp file 
pngmap.save(bmpname, 'bmp')
#remove the .png file
subprocess.call(["rm", jpgname])
=======
pngmap = Image.open(pngname)
#save the Image as a .bmp file 
pngmap.save(bmpname)
#remove the .png file
subprocess.call(["rm", pngname])
>>>>>>> 94938cdc9e6390c12af0013c86aea31433c12796
