import os
from flask import Flask, redirect, url_for, request, render_template, jsonify
import json
import requests

app = Flask(__name__)


@app.route('/')
def home_page():
    output = "hello"
    r = requests.get('http://172.17.0.4:5000/api/v1.0/telemetry')
    output = json.dumps(r.text)
    return render_template("index.html",output=output)


if __name__ == "__main__":
    app.run(host='0.0.0.0', port=8080, debug=True)
