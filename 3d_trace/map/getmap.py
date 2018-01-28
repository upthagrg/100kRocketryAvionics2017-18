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
	url = url + "&zoom=14&size=512x512"
	return url

#define names and paths for images
pngname = '../resources/textures/map.png'
bmpname = '../resources/textures/map.bmp'
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
mapfile = open(pngname ,'wb')
#write the binary map to the .png
mapfile.write(mapbin.content)
#close the .png file
mapfile.close()
#open the .png as an Image
pngmap = Image.open(pngname)
#save the Image as a .bmp file 
pngmap.save(bmpname)
#remove the .png file
subprocess.call(["rm", pngname])
