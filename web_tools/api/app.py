import os
from flask import Flask, redirect, url_for, request, render_template, jsonify
import pymongo
from pymongo import MongoClient
from flask_cors import CORS
import json
from datetime import datetime

app = Flask(__name__)
CORS(app)
client = MongoClient(os.environ['DB_PORT_27017_TCP_ADDR'], 27017)
db = client.rocketinfodb


@app.route('/api/v1.0/telemetry_all', methods=['GET'])
def get_all_telemetry():
    telemetry = db.telemetry
    if telemetry.count() == 0:
        return jsonify({'result': 'no data'})
    output = []
    for t in telemetry.find():
        output.append({
            'altitude': t['altitude'],
            'longitude': t['longitude'],
            'latitude': t['latitude'],
            'time': t['time'],
            'velocity': t['velocity']
        })
    return jsonify({'result': output})

@app.route('/api/v1.0/telemetry', methods=['GET'])
def get_telemetry():
    telemetry = db.telemetry
    if telemetry.count() == 0:
        return jsonify({'result': 'no data'})
    output = []
    cursor = telemetry.find().sort([("_id", pymongo.DESCENDING)])
    output.append({
        'altitude': cursor[0]['altitude'],
        'longitude': cursor[0]['longitude'],
        'latitude': cursor[0]['latitude'],
        'time': cursor[0]['time'],
        'velocity': cursor[0]['velocity']
    })
    return jsonify({'result': output})


@app.route('/api/v1.0/start_launch', methods=['GET'])
def start_launch():
    start_time = "{'start_time' : '" + datetime.now().strftime(
        '%Y-%m-%d %H:%M:%S') + "'}"
    _id = db.launch.insert_one(start_time)
    return "Launch has started at: " + start_time + " (" + _id + ")"


#TODO Fix launch data
@app.route('/api/v1.0/launch', methods=['GET'])
def get_launch_data():
    _launchdata = db.launch
    if _launchdata.count() == 0:
        return jsonify({'result': 'no data'})
    output = []
    for t in _launchdata.find():
        output.append({
            'altitude': t['altitude'],
            'longitude': t['longitude'],
            'latitude': t['latitude'],
            'time': t['time'],
            'velocity': t['velocity']
        })
    return jsonify({'result': output})


@app.route('/api/v1.0/end_launch', methods=['GET'])
def end_launch():
    end_time = "{'end_time' : '" + datetime.now().strftime(
        '%Y-%m-%d %H:%M:%S') + "'}"
    _id = db.launch.insert_one(end_time)
    return "Launch has ended at: " + end_time +" ("+_id + ")"


@app.route('/api/v1.0/telemetry', methods=['POST'])
def post_telemetry():
    _telemetry = request.json
    _id = db.telemetry.insert_one(_telemetry)
    return "(" + _id + ")"


if __name__ == "__main__":
    app.run(host='0.0.0.0', debug=True)
