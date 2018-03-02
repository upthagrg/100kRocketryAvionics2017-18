import os
from flask import Flask, redirect, url_for, request, render_template, jsonify
import pymongo
from pymongo import MongoClient
from bson import ObjectId
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

@app.route('/api/v1.0/telemetry/', methods=['GET'])
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
    app.run(host='0.0.0.0', debug=True)
