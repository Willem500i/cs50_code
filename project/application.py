import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from urllib.request import urlopen
import json
import requests
import urllib.parse

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///cards.db")

@app.route("/", methods=["GET"])
def index():
    return render_template('index.html')


@app.route("/report", methods=["GET", "POST"])
def report():

    if request.method == "GET":
        return render_template('report.html',gpus = db.execute("SELECT * FROM average ORDER BY card"))

    if request.method == "POST":

        core = request.form.get("core")
        mem = request.form.get("mem")
        pow = request.form.get("pow")
        model = request.form.get("gpu")
        hash = request.form.get("MH")
        card = request.form.get("model")
        miner = request.form.get("miner")

        db.execute("INSERT INTO responses (core, mem, pow, model, hash, card,miner) VALUES(?,?,?,?,?,?,?)", core,mem,pow,model,hash,card,miner)

        current = db.execute("SELECT * FROM average WHERE card=?",model)


        #try:
        newcore = (int(core) + int(current[0]['core']))/2
        newmem = (int(mem) + int(current[0]['mem']))/2
        newpow = (int(pow) + int(current[0]['pow']))/2
        newhash = (int(hash) + int(current[0]['hash']))/2

        if percentdiff(newcore, int(current[0]['core'])) < 30 and percentdiff(newmem, int(current[0]['mem'])) < 30 and percentdiff(newpow, int(current[0]['pow'])) < 30 and percentdiff(newhash, int(current[0]['hash'])) < 30:
            db.execute("UPDATE average SET core=?,mem=?,pow=?,hash=? WHERE card=?", newcore,newmem,newpow,newhash,model)
        return redirect("/")
        #except:
          #  return redirect("/")


@app.route("/profitability", methods=["GET", "POST"])
def profitability():

    if request.method == "GET":

        hr = 0
        p = 0
        fee = 0
        ecost = 0.1
        hcost = 0

        return render_template('calculate.html',gpus = db.execute("SELECT * FROM average ORDER BY card"),hr=0,p=0,ecost=0.1,hcost=0,quote=0,stats=0)

    if request.method == "POST":

        card = request.form.get("gpu")

        if (card == "custom"):

            hr = request.form.get("MH")
            p = request.form.get("pow")
            fee = 0
            ecost = request.form.get("ecost")
            hcost = request.form.get("hcost")

            url = f"https://whattomine.com/coins/151.json?hr={str(hr)}&p={str(p)}&fee={str(fee)}&cost={str(ecost)}&hcost={str(hcost)}&span_br=1h&span_d=24"
            response = requests.get(url)
            quote = response.json()

            revenue = quote["revenue"]
            cost = quote["cost"]
            profit = quote["profit"]

            if not float(profit[1:]) == 0:
                breakeven = float(hcost) / float(profit[1:])
            else:
                breakeven = 0

            return render_template('calculate.html',gpus = db.execute("SELECT * FROM average ORDER BY card"),quote=quote,stats=0,breakeven=breakeven,hr=hr,p=p,ecost=ecost,hcost=hcost)

        else:
            stats = db.execute("SELECT * FROM average WHERE card=?", card)

            hr = stats[0]['hash']
            p = stats[0]['pow']
            fee = 0
            ecost = request.form.get("ecost")
            hcost = request.form.get("hcost")

            url = f"https://whattomine.com/coins/151.json?hr={str(hr)}&p={str(p)}&fee={str(fee)}&cost={str(ecost)}&hcost={str(hcost)}&span_br=1h&span_d=24"
            response = requests.get(url)
            quote = response.json()

            revenue = quote["revenue"]
            cost = quote["cost"]
            profit = quote["profit"]

            if not float(profit[1:]) == 0:
                breakeven = float(hcost) / float(profit[1:])
            else:
                breakeven = 0

            return render_template('calculate.html',gpus = db.execute("SELECT * FROM average ORDER BY card"),breakeven=breakeven,quote=quote,stats=stats,hr=hr,p=p,ecost=ecost,hcost=hcost)


def usd(value):
    """Format value as USD."""
    return f"${value:,.2f}"

def percentdiff(value1, value2):

    return 100 * abs(value1 - value2)/((value1 + value2)/2)