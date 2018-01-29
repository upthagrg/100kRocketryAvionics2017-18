import os
from flask import Flask, redirect, url_for, request, render_template, jsonify
import json
import requests

app = Flask(__name__)


@app.route('/')
def home_page():
    r = requests.get('http://172.17.0.4:5000/api/v1.0/telemetry')
    telemetry = json.loads(r.text)['result']
    return render_template("index.html", telemetry=telemetry)


if __name__ == "__main__":
    app.run(host='0.0.0.0', port=8080, debug=True, threaded=True)
