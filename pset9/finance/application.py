import os
from datetime import datetime

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

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


# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    id = session["user_id"]
    holdings = db.execute("SELECT * FROM holdings WHERE id=?", id)

    # total is cash balance
    total = (db.execute("SELECT * FROM users WHERE id=?", session["user_id"]))[0]['cash']

    # import lookup into jinja to use in index.html
    app.jinja_env.globals.update(lookup=lookup)

    tickers = db.execute("SELECT * FROM holdings WHERE id=?", id)

    # add balances of all tickers owned to total
    for i in range(len(tickers)):
        total += lookup(tickers[i]['ticker'])['price'] * tickers[i]['amount']

    return render_template("index.html", total=total, holdings=holdings, cash=(db.execute("SELECT * FROM users WHERE id=?", session["user_id"]))[0]['cash'])


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    # when GET
    if request.method == "GET":
        return render_template("buy.html")

    # when POST
    if request.method == "POST":

        ticker = request.form.get("symbol").upper()

        try:
            number = int(request.form.get("shares"))
        except ValueError:
            return apology("Share count must be an integer",400)

        # check if ticker exists
        if lookup(ticker) == None:
            return apology("Ticker does not exist",400)

        # check if they can afford the stock
        price = float(lookup(ticker)['price'])

        cash = (db.execute("SELECT * FROM users WHERE id=?", session["user_id"]))[0]['cash']

        transaction = price * number

        if (0 >= number):
            return apology("Invalid share amount",400)

        if transaction > cash:
            return apology("Insufficient Funds",400)

        # insert purchase into holdings. If it already exsists, update holdings amount
        db.execute("INSERT INTO holdings (id, ticker, amount) VALUES (?,?,?) ON CONFLICT (id,ticker) DO UPDATE SET amount= amount + ?", session["user_id"],ticker,number,number)

        # take away their money
        newcash = cash - transaction

        db.execute("UPDATE users SET cash=? WHERE id=?", newcash,session["user_id"])

        # record transaction
        company = lookup(ticker)['name']
        now = datetime.now()
        timestamp = now.strftime("%d/%m/%Y %H:%M:%S")

        db.execute("INSERT INTO transactions (user_id, symbol, company, shares, price, timestamp) VALUES (?,?,?,?,?,?)",session["user_id"],ticker,company,number,price,timestamp)

        flash("Bought!")
        return redirect("/")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    # display table with a history of all transactions

    id = session["user_id"]
    transactions = db.execute("SELECT * FROM transactions WHERE user_id=?", id)

    return render_template("history.html", transactions=transactions)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    # When POST
    if request.method == "POST":

        # Lookup stock symbol by calling the lookup function
        ticker = request.form.get("symbol")

        if lookup(ticker) == None:
            return apology("Ticker does not exist",400)

        # Display results
        return render_template("quoteout.html", name = lookup(ticker)['name'], price = lookup(ticker)['price'], ticker = lookup(ticker)['symbol'])

    # When GET
    if request.method == "GET":
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # When requested via GET, display registration form
    if request.method == "GET":
        return render_template("register.html")

    # When requested via POST, insert new user into users table
    if request.method == "POST":

        # store username, password, and test for easier use

        username = request.form.get("username")
        password = request.form.get("password")
        test = request.form.get("confirmation")

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 400)

        # Ensure password confirm was submitted
        elif not request.form.get("confirmation"):
            return apology("must confirm password", 400)

        # Ensure passwords match
        elif not (password == test):
            return apology("Passwords much match", 400)

        # Query database for username repeats
        if db.execute("SELECT * FROM users WHERE username = ?", username):
            return apology("Username already exists", 400)

        # Check for password security
        if (len(request.form.get("password")) < 8):
            return apology("Password must be at least 8 characters long")

        # Create password hash
        hash = generate_password_hash(request.form.get("password"))

        # Save password hash with username
        db.execute("INSERT INTO users (username, hash) VALUES (?,?)", username, hash)

        # Log user in
        rows = db.execute("SELECT * FROM users WHERE username = ?", username)
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    # if GET, display form to sell
    if request.method == "GET":

        id = session["user_id"]
        holdings = db.execute("SELECT * FROM holdings WHERE id=?", id)

        return render_template("sell.html",holdings=holdings)

    # when POST
    if request.method == "POST":

        ticker = request.form.get("symbol")

        # check if they can afford the stock
        price = float(lookup(ticker)['price'])
        number = float(request.form.get("shares"))

        if not number:
            return apology("Must submit share count")

        cash = (db.execute("SELECT * FROM users WHERE id=?", session["user_id"]))[0]['cash']

        transaction = price * number

        sharecount = db.execute("SELECT * FROM holdings WHERE id=? AND ticker=?", session["user_id"], ticker)[0]['amount']

        if number > sharecount:
            return apology("Insufficient Selling Power")

        if number <= 0:
            return apology("Invalid share amount")

        # Remove sold shares from holdings
        db.execute("UPDATE holdings SET amount = amount - ? WHERE id=? AND ticker=?", number,session["user_id"],ticker)

        # delete holding if is 0
        if db.execute("SELECT * FROM holdings WHERE id=? AND ticker=?", session["user_id"], ticker)[0]['amount'] == 0:
            db.execute("DELETE FROM holdings WHERE id=? AND ticker=?", session["user_id"],ticker)

        # update cash
        newcash = cash + transaction
        db.execute("UPDATE users SET cash=? WHERE id=?", newcash,session["user_id"])

        # record transaction
        company = lookup(ticker)['name']
        now = datetime.now()
        timestamp = now.strftime("%d/%m/%Y %H:%M:%S")

        db.execute("INSERT INTO transactions (user_id, symbol, company, shares, price, timestamp) VALUES (?,?,?,?,?,?)",session["user_id"],ticker,company,(number * -1),price,timestamp)

        flash("Sold!")
        return redirect("/")


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
