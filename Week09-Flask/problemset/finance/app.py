import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    rows = db.execute("SELECT * FROM stocks WHERE user_id=?", session["user_id"])
    cash = db.execute("SELECT cash FROM users WHERE id=?", session["user_id"])[0][
        "cash"
    ]
    total_all = cash
    data = []
    for row in rows:
        symbol = row["symbol"]
        shares = row["shares"]
        name, price, _ = lookup(symbol).values()
        total = shares * price
        price = usd(price)
        total_all += total
        total = usd(total)
        data.append(
            {
                "symbol": symbol,
                "name": name,
                "shares": shares,
                "price": price,
                "total": total,
            }
        )

    return render_template(
        "index.html", data=data, cash=usd(cash), total_all=usd(total_all)
    )


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        # Validate the input
        symbol = request.form.get("symbol")
        number = request.form.get("shares")
        if not symbol:
            return apology("must provide symbol", 403)

        # Check for invalid symbol
        data = lookup(symbol)
        if not data:
            return apology("invalid symbol", 403)

        # Ensure shares is a valid number
        if not number:
            return apology("must provide number", 403)
        if not number.isdigit() or (number.isdigit() and int(number) < 0):
            return apology("invalid number", 403)

        # Get user's cash as float
        rows = db.execute("SELECT * FROM users WHERE id=?", session["user_id"])
        cash = rows[0]["cash"]

        # total value of shares
        number = int(number)
        total = number * data["price"]

        # Check if user can buy it
        if total > cash:
            return apology("Not enough cash", 403)

        # Else buy it
        cash -= total

        # Updating the databases

        db.execute("UPDATE users SET cash = ? WHERE id = ?", cash, session["user_id"])

        # check if this stock is already bought, if yes update else insert
        rows = db.execute(
            "SELECT * FROM stocks WHERE user_id = ? AND symbol=?",
            session["user_id"],
            symbol,
        )
        if len(rows):
            db.execute(
                "UPDATE stocks SET shares = ? WHERE user_id = ? AND symbol=?",
                rows[0]["shares"] + number,
                session["user_id"],
                symbol,
            )
        else:
            db.execute(
                "INSERT INTO stocks (user_id, symbol, shares) VALUES (?, ?, ?)",
                session["user_id"],
                symbol,
                number,
            )

        # Update transaction history
        db.execute(
            "INSERT INTO history (id, symbol, shares, price, action, date) VALUES (?, ?, ?, ?, 'buy', datetime('now'))",
            session["user_id"],
            symbol,
            number,
            data["price"],
        )

        flash("Bought!")

        return redirect("/")

    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    return apology("TODO")


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
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
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
    if request.method == "POST":
        symbol = request.form.get("symbol")
        if not symbol:
            return apology("must provide symbol", 403)
        data = lookup(symbol)
        data["price"] = usd(data["price"])
        if not data:
            return apology("invalid symbol", 403)
        return render_template("show_quote.html", data=data)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":
        username = request.form.get("username")
        password = request.form.get("password")
        confirm = request.form.get("confirm-password")

        # Validations
        if not username:
            return apology("must provide username", 403)
        if not password or not confirm:
            return apology("must provide password", 403)
        if password != confirm:
            return apology("password didn't match", 403)

        # Check for username already in use
        rows = db.execute("SELECT * FROM users WHERE username = ?", username)
        if len(rows) > 0:
            return apology("username already taken", 403)

        # Generate password hash
        password = generate_password_hash(password)

        # Insert into database
        db.execute(
            "INSERT INTO users (username, hash) VALUES(?, ?)", username, password
        )

        # Login user and redirect to homepage
        rows = db.execute("SELECT * FROM users WHERE username=?", username)
        session["user_id"] = rows[0]["id"]
        return redirect("/")

    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    rows = db.execute("SELECT symbol FROM stocks WHERE user_id=?", session["user_id"])
    if request.method == "POST":
        symbol = request.form.get("symbol")
        number = request.form.get("shares")
        # Validations
        if not symbol:
            return apology("must provide symbol", 403)

        # Ensure shares is a valid number
        if not number:
            return apology("must provide number", 403)
        if not number.isdigit() or (number.isdigit() and int(number) < 0):
            return apology("invalid number", 403)
        number = int(number)

        # Check if user has that stock and number is less than equal to available shares
        rows = db.execute(
            "SELECT shares FROM stocks WHERE symbol=? AND user_id=?",
            symbol,
            session["user_id"],
        )
        if len(rows) < 1:
            return apology("no stocks available")
        shares = rows[0]["shares"]
        if shares < number:
            return apology("Can' sell these many shares", 403)

        # Sell it
        price = lookup(symbol)["price"]
        # Update user's cash and shares
        shares -= number
        cash = db.execute("SELECT cash FROM users WHERE id=?", session["user_id"])[0][
            "cash"
        ]
        cash += number * price
        db.execute("UPDATE users SET cash=? WHERE id=?", cash, session["user_id"])

        if shares == 0:
            # Delete that stock from table
            db.execute(
                "DELETE FROM stocks WHERE symbol = ? AND user_id = ?",
                symbol,
                session["user_id"],
            )
        else:
            db.execute(
                "UPDATE stocks SET shares=? WHERE symbol=? AND user_id=?",
                shares,
                symbol,
                session["user_id"],
            )

        # Update transaction history
        db.execute(
            "INSERT INTO history (id, symbol, shares, price, action, date) VALUES (?, ?, ?, ?, 'sell', datetime('now'))",
            session["user_id"],
            symbol,
            shares,
            price,
        )

        flash("Sold!")

        return redirect("/")

    else:
        return render_template("sell.html", rows=rows)
