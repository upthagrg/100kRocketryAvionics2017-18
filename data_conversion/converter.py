#!/bin/python
import json, argparse, time
from datetime import datetime, timedelta

parser = argparse.ArgumentParser()
parser.add_argument('--type', help='enter stage')
parser.add_argument('--file', help='enter stage')
args = parser.parse_args()

ptime = 0.0
pdtime = 0.0
palt = 0.0
flag = 0
ctime = datetime.now()

with open(args.file, 'r') as f:
    for line in f:
        if "GGA," in line[:7]:
            gga_data = line.split(",")
            if len(gga_data) != 15:
                continue
            data = {}
            if flag == 0:
                data["velocity"] = str(pdtime)
                palt = gga_data[9]
                flag = 1
            else:
                calt = float(gga_data[9]) - float(palt)
                ctime = ptime
                palt = gga_data[9]

                ntime = datetime.strptime(
                    str(ctime.year) + " " + str(ctime.month) + " " +
                    str(ctime.day) + " " + str(gga_data[1].split(".")[0]),
                    '%Y %m %d %H%M%S')
                cstime = (ntime - ptime).total_seconds()
                cdtime = cstime - pdtime
                data["velocity"] = str(calt / cdtime)
                pdtime = (ntime - ptime).total_seconds()
            data["latitude"] = gga_data[2]
            data["longitude"] = "-" + gga_data[4]
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
            print json_data





#$GNGGA,005823.000,4434.1767,N,12316.5159,W,1,10,1.02,70.4,M,-21.0,M,,*7C
