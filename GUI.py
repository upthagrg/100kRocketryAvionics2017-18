# for Python2
import Tkinter as tk   ## notice capitalized T in Tkinter 
import tkMessageBox as tkm
	
from PIL import ImageTk,Image
import sys
import os
import subprocess


#print current Python Version
print(sys.version)

#define sim flight callback function
def simcallback():
	os.chdir('./data_handling')
	subprocess.call(['./datahandle', '-sim'])
#define Tracking callback function
def monitorccallback():
	#only use if you know what you are doing
	os.chdir('./data_handling')
	subprocess.call(['./datahandle', '-dock', '-fifo'])
#define Logging callback function
def logscallback():
	dirfile = open('/HART_DIR.txt', 'r')
	string1 = dirfile.read()
	string2 = string1[:-1]
	os.chdir(string2)
	root2 = tk.Tk()
	root2.title("View Logs")
	root2.minsize(width=480, height=480)
	root2.maxsize(width=1920, height=1080)
	root2.resizable(width=True, height=True)
	text1 = tk.Text(root2)
	for filename in os.listdir("./logs"):
		text1.insert('end', filename)
		text1.insert('end', '\n')
		myfile = open("./logs/"+filename, 'r')
		string = myfile.read()
		text1.insert('end', string)
	text1.pack()
	root2.mainloop()

#change directory
dirfile = open('/HART_DIR.txt', 'r')
string1 = dirfile.read()
string2 = string1[:-1]
os.chdir(string2)
#define gui
root = tk.Tk()
root.title("HART Avionics")
root.minsize(width=480, height=480)
root.maxsize(width=1920, height=1080)
root.resizable(width=True, height=True)

#sim button
simbtn = tk.Button(root, text="Sim Flight", command=simcallback, bd=4)
simbtn.pack()
simbtn.place(relx=0.5, rely=0.4, width=128, x=-64)
#monitor button
monbtn = tk.Button(root, text="Monitor Flight", command=monitorccallback, bd=4)
monbtn.pack()
monbtn.place(relx=0.5, rely=0.5, width=128, x=-64)
#view logs
logbtn = tk.Button(root, text="View logs", command=logscallback, bd=4)
logbtn.pack()
logbtn.place(relx=0.5, rely=0.6, width=128, x=-64)
#start main UI loop
root.mainloop()

