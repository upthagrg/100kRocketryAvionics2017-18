import os
from flask import Flask, redirect, url_for, request, render_template, jsonify
import pymongo
from pymongo import MongoClient
from flask_cors import CORS
import json
import hashlib

app = Flask(__name__)
CORS(app)
client = MongoClient(os.environ['DB_PORT_27017_TCP_ADDR'], 27017)
db = client.telemetrydb


@app.route('/api/v1.0/telemetry_all', methods=['GET'])
def get_all_telemetry():
    telemetry = db.telemetrydb
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
    telemetry = db.telemetrydb
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


@app.route('/api/v1.0/telemetry', methods=['POST'])
def post_telemetry():
    _telemetry = request.json
    db.telemetrydb.insert_one(_telemetry)
    hash_object = hashlib.md5(str(_telemetry))
    hash_object.hexdigest()
    return "Success:" + hash_object.hexdigest()


if __name__ == "__main__":
    app.run(host='0.0.0.0', debug=True)
