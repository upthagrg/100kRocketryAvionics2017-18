#!/bin/python
import json, argparse, time
from datetime import datetime, timedelta

parser = argparse.ArgumentParser()
parser.add_argument('--type', help='enter stage')
parser.add_argument('--file', help='enter stage')
args = parser.parse_args()

list_of_good_chars = {'1','2','3','4','5','6','7','8','9','0','-','.'}
ptime = 0.0
pdtime = 0.0
palt = 0.0
flag = 0
ctime = datetime.now()
bflag = 0

with open(args.file, 'r') as f:
    for line in f:
        if "GGA," in line:
            bflag = 0
            gga_array = line.split("$")
            for item in gga_array:
                if "GGA," in item:
                    gga_data = item.split(",")
                    if gga_data[1] == '' or gga_data[2] == '' or gga_data[4] == '' or gga_data[9] == '':
                        print("packet rejected, missing data")
                        continue
                    if len(gga_data) != 15:
                        print("packet rejected, missing data")
                        continue
                    data = {}
                    if flag == 0:
                        data["velocity"] = str(pdtime)
                        palt = gga_data[9]
                        flag = 1
                    else:
                        for c in gga_data[9]:
                         if c not in list_of_good_chars:
                          print ("packet rejectd, corrupted data")
                          bflag = 1
                        if bflag == 1:
                         continue
                        calt = float(gga_data[9]) - float(palt)
                        ctime = ptime
                        palt = gga_data[9]
                        for c in gga_data[1]:
                         if c not in list_of_good_chars:
                          print ("packet rejectd, corrupted data")
                          bflag = 1
                        if bflag == 1:
                         continue

                        ntime = datetime.strptime(
                            str(ctime.year) + " " + str(ctime.month) + " " +
                            str(ctime.day) + " " + str(gga_data[1].split(".")[0]),
                            '%Y %m %d %H%M%S')
                        cstime = (ntime - ptime).total_seconds()
                        cdtime = cstime - pdtime
                        data["velocity"] = str(calt / cdtime)
                        pdtime = (ntime - ptime).total_seconds()
                    for c in gga_data[2]:
                     if c not in list_of_good_chars:
                      print ("packet rejectd, corrupted data")
                      bflag = 1
                    if bflag == 1:
                     continue
                    converted = float(gga_data[2])
                    converted2 = converted
                    if converted > 10000.0:
                        converted = converted % 10000.0
                    if converted > 1000.0:
                        converted = converted % 1000.0
                    if converted > 100.0:
                        converted = converted % 100.0
                    converted2 -= converted
                    converted2 /= 100.00
                    converted2 += converted/60.0
                    #data["latitude"] = gga_data[2]
                    data["latitude"] = "%.5f" % converted2
                    for c in gga_data[4]:
                     if c not in list_of_good_chars:
                      print ("packet rejectd, corrupted data")
                      bflag = 1
                    if bflag == 1:
                     continue
                    converted = float(gga_data[4])
                    converted2 = converted
                    if converted > 10000.0:
                        converted = converted % 10000.0
                    if converted > 1000.0:
                        converted = converted % 1000.0
                    if converted > 100.0:
                        converted = converted % 100.0
                    converted2 -= converted
                    converted2 /= 100.00
                    converted2 += converted/60.0
                    #data["longitude"] = "-" + gga_data[4]
                    data["longitude"] = "-" + '%.5f' % converted2
                    data["altitude"] = gga_data[9]
                    if ptime == 0.0:
                        ptime = datetime.strptime(
                            str(ctime.year) + " " + str(ctime.month) + " " +
                            str(ctime.day) + " " + str(gga_data[1].split(".")[0]),
                            '%Y %m %d %H%M%S')
                        data["time"] = 0
                    else:
                        ntime = datetime.strptime(
                            str(ctime.year) + " " + str(ctime.month) + " " +
                            str(ctime.day) + " " + str(gga_data[1].split(".")[0]),
                            '%Y %m %d %H%M%S')
                        data["time"] = (ntime - ptime).total_seconds()
                    data["type"] = args.type
                    json_data = json.dumps(data)
                    print json_data + "&&"
print "**&&"




#$GNGGA,005823.000,4434.1767,N,12316.5159,W,1,10,1.02,70.4,M,-21.0,M,,*7C
