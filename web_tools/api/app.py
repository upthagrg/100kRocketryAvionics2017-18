import os
from flask import Flask, redirect, url_for, request, render_template, jsonify
import pymongo
from pymongo import MongoClient
from bson import ObjectId
from flask_cors import CORS
import json
from datetime import datetime, timedelta

app = Flask(__name__)
CORS(app)
client = MongoClient(os.environ['DB_PORT_27017_TCP_ADDR'], 27017)
db = client.rocketinfodb


@app.route('/api/v1.0/telemetry_all', methods=['GET'])
def get_all_telemetry():
    telemetry = db.telemetry
    launch = db.launch
    if telemetry.count() == 0:
        return jsonify({'result': 'no data'})
    booster = []
    sustainer = []
    for t in telemetry.find():
        cursor = launch.find().sort([("_id", pymongo.DESCENDING)])
        dt = datetime.strptime(cursor[0]['start_time'],
                                                '%Y-%m-%d %H:%M:%S')
        nt = dt + timedelta(seconds=float(t['time']))
        time = format(nt, '%Y-%m-%d %H:%M:%S')
        if t['type'] == "b":
            booster.append({
                'altitude': t['altitude'],
                'longitude': t['longitude'],
                'latitude': t['latitude'],
                'time': time,
                'velocity': t['velocity'],
                'type': t['type']
            })
        else:
            sustainer.append({
                'altitude': t['altitude'],
                'longitude': t['longitude'],
                'latitude': t['latitude'],
                'time': time,
                'velocity': t['velocity'],
                'type': t['type']
            })
    return jsonify({'booster': booster},{'sustainer':sustainer})

@app.route('/api/v1.0/telemetry/<t_type>', methods=['GET'])
def get_telemetry(t_type):
    telemetry = db.telemetry
    if telemetry.count() == 0:
        return jsonify({'result': 'no data'})
    output = []
    cursor = telemetry.find({"type": t_type}).sort([("_id", pymongo.DESCENDING)])
    output.append({
        'altitude': cursor[0]['altitude'],
        'longitude': cursor[0]['longitude'],
        'latitude': cursor[0]['latitude'],
        'time': cursor[0]['time'],
        'velocity': cursor[0]['velocity'],
        'type': cursor[0]['type']
    })
    return jsonify({'result': output})


@app.route('/api/v1.0/launch/', methods=['GET'])
def get_launch():
    launch = db.launch
    if launch.count() == 0:
        return jsonify({'result': 'no data'})
    output = []
    cursor = launch.find().sort([("_id", pymongo.DESCENDING)])
    output.append({
        'start_time': cursor[0]['start_time'],
        'end_time': cursor[0]['end_time']
    })
    return jsonify({'result': output})


@app.route('/api/v1.0/start_launch/<launch_id>', methods=['GET'])
def start_launch(launch_id):
    if launch_id == "":
        return "No launch id provided."
    start_time = str(datetime.now().strftime("%Y-%m-%d %H:%M:%S"))
    db.launch.update_one(
        {"_id": ObjectId(launch_id)},
        {
        "$set": {
            "start_time": start_time
        }
        }
    )
    return "Launch has started at: " + str(start_time)

@app.route('/api/v1.0/end_launch/<launch_id>', methods=['GET'])
def end_launch(launch_id):
    if launch_id == "":
        return "No launch id provided."
    end_time = str(datetime.now().strftime("%Y-%m-%d %H:%M:%S"))
    db.launch.update_one(
        {"_id": ObjectId(launch_id)},
        {
        "$set": {
            "end_time":end_time
        }
        }
    )
    return "Launch has ended at: " + str(end_time)

@app.route('/api/v1.0/prepare_launch', methods=['GET'])
def get_launch_data():
    data = {"start_time" : "", "end_time" : ""}
    _id = db.launch.insert_one(data).inserted_id
    return str(_id)

@app.route('/api/v1.0/telemetry', methods=['POST'])
def post_telemetry():
    _telemetry = request.json
    _id = db.telemetry.insert_one(_telemetry).inserted_id
    return "(" + str(_id) + ")"


if __name__ == "__main__":
    app.run(host='0.0.0.0', debug=True, threaded=True)
