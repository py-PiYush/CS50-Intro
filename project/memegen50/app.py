from flask import Flask, render_template, request, redirect, session
import requests
from helpers import get_memes, apology, login_required

# from cs50 import SQL
import sqlite3 as s
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash

# Configure application
app = Flask(__name__)

# Configure sessions
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure cs50 library to use sqlite database
# db = SQL("sqlite:///project.db")

# con = s.connect("project.db")
# db = con.cursor()


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# Set of allowed subreddits
SUBS = {
    "ProgrammerHumor",
    "wholesomememes",
    "programmerreactions",
    "linuxmemes",
    "programmingmemes",
}


@app.route("/")
@login_required
def index():
    return render_template("index.html", subs=SUBS)


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register User"""
    # User reached via POST request
    if request.method == "POST":
        with s.connect("project.db") as con:
            db = con.cursor()
            # Get form inputs
            username = request.form.get("username")
            password = request.form.get("password")
            confirm = request.form.get("confirm-password")

            # Validations
            if not username:
                return apology("must provide username", 400)
            if not password or not confirm:
                return apology("must provide password", 400)
            if password != confirm:
                return apology("password didn't match", 400)

            # Check for username already in use
            rows = db.execute(
                "SELECT * FROM users WHERE username = ?", (username,)
            ).fetchall()
            if len(rows) > 0:
                return apology("username already taken", 400)

            # Generate password hash
            password = generate_password_hash(password)

            # Insert into database
            db.execute(
                "INSERT INTO users (username, hash) VALUES(?, ?)", (username, password)
            )
            con.commit()

            # Login user and redirect to homepage
            rows = db.execute(
                "SELECT * FROM users WHERE username=?", (username,)
            ).fetchall()
            session["user_id"] = rows[0][0]
            return redirect("/")

    # User reached iva GET request
    else:
        # Show user register form
        return render_template("register.html")


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""
    session.clear()

    # User reached via POST request
    if request.method == "POST":
        with s.connect("project.db") as con:
            db = con.cursor()
            # Get and validate username
            username = request.form.get("username")
            if not username:
                return apology("must provide username")

            # Get and validate password
            password = request.form.get("password")
            if not password:
                return apology("must provide password")

            # Query database for this username
            rows = db.execute(
                "SELECT * FROM users WHERE username=?", (username,)
            ).fetchall()

            # Validate Username and Password
            if len(rows) != 1:
                return apology("Invalid username ")
            password_hash = rows[0][2]
            if not check_password_hash(password_hash, password):
                return apology("Invalid password")

            # Log user in
            session["user_id"] = rows[0][0]

            # Redirect to home page
            return redirect("/")

    # User reached via GET request
    else:
        # Show login form
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/login")


@app.route("/memegen", methods=["GET", "POST"])
@login_required
def memegen():
    """Get memes from subreddit"""
    # User reached via POST request
    contents = []
    if request.method == "POST":
        # Get and validate subreddit name
        subreddit = request.form.get("subreddit")
        if not subreddit or subreddit not in SUBS:
            return apology("Invalid subreddit")

        # Get and validate count(max 50)
        count = request.form.get("count")
        print(count)
        if not count:
            return apology("Must provide count")
        if not (0 < int(count) <= 50):
            return apology("Invalid count")

        # Get memes via API call
        contents = get_memes(subreddit, count)
        if not contents:
            return apology("Not found", 404)

        # Show memes
        return render_template("memes.html", contents=contents)

    # User reached via GET (manually entering the link)

    # Redirect to home page
    return redirect("/")


@app.route("/add", methods=["GET", "POST"])
@login_required
def add():
    # If post request
    if request.method == "POST":
        # Get data
        subreddit = request.form.get("subreddit")
        title = request.form.get("title")
        image = request.form.get("image")

        # Validate
        if not subreddit or not title or not image:
            return apology("bad request")

        if subreddit not in SUBS:
            return apology("Invalid subreddit")
        with s.connect("project.db") as con:
            db = con.cursor()
            # Check if already exists
            rows = db.execute(
                "SELECT * FROM collections WHERE image=?", (image,)
            ).fetchall()
            if len(rows) > 0:
                return apology("Already saved")

            # Insert into database
            db.execute(
                "INSERT INTO collections(subreddit, title, image, link, user_id) VALUES(?,?,?,?,?)",
                (subreddit, title, image, "#", session["user_id"]),
            )
            con.commit()
        return redirect("/memegen")


@app.route("/show")
@login_required
def show():
    """Shows user's collection of memes"""
    # Get contents from database
    with s.connect("project.db") as con:
        db = con.cursor()
        rows = db.execute(
            "SELECT * FROM collections WHERE user_id=?", (session["user_id"],)
        ).fetchall()
        return render_template("show.html", contents=rows)


@app.route("/remove/<int:id>")
@login_required
def remove(id):
    """Removes a meme from collection"""
    with s.connect("project.db") as con:
        db = con.cursor()
        # Query database for the given id to check if it exists
        rows = db.execute("SELECT * FROM collections WHERE id=?", (id,)).fetchall()
        if len(rows) == 0:
            return apology("bad request")

        # Delete
        db.execute("DELETE FROM collections WHERE id=?", (id,))
        con.commit()
        return redirect("/show")


# con.close()
