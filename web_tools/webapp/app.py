import os
from flask import Flask, redirect, url_for, request, render_template, jsonify
from pymongo import MongoClient

app = Flask(__name__)

client = MongoClient(os.environ['DB_PORT_27017_TCP_ADDR'], 27017)
db = client.telemetrydb


@app.route('/api/v1.0/telemetry', methods=['GET'])
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


@app.route('/api/v1.0/telemetry', methods=['POST'])
def post_telemetry():
    _telemetry = request.json
    db.telemetrydb.insert_one(_telemetry)
    return "Added."


if __name__ == "__main__":
    app.run(host='0.0.0.0', debug=True)
