import os
import requests

from flask import redirect, render_template, session
from functools import wraps


def apology(message, code=400):
    """Render message as an apology to user."""

    def escape(s):
        """
        Escape special characters.

        https://github.com/jacebrowning/memegen#special-characters
        """
        for old, new in [
            ("-", "--"),
            (" ", "-"),
            ("_", "__"),
            ("?", "~q"),
            ("%", "~p"),
            ("#", "~h"),
            ("/", "~s"),
            ('"', "''"),
        ]:
            s = s.replace(old, new)
        return s

    return render_template("apology.html", top=code, bottom=escape(message)), code


def login_required(f):
    """
    Decorate routes to require login.

    https://flask.palletsprojects.com/en/1.1.x/patterns/viewdecorators/
    """

    @wraps(f)
    def decorated_function(*args, **kwargs):
        if session.get("user_id") is None:
            return redirect("/login")
        return f(*args, **kwargs)

    return decorated_function


def get_memes(subreddit, count):
    """Return 'count' number of memes from 'subreddit'"""

    # Contact API
    try:
        url = f"https://meme-api.com/gimme/{subreddit}/{count}"
        response = requests.get(url)
        response.raise_for_status()
    except requests.RequestException:
        return None

    # Parse response
    try:
        memes = response.json()
        contents = []
        for meme in memes["memes"]:
            d = {
                "subreddit": meme["subreddit"],
                "title": meme["title"],
                "image": meme["preview"][-1],
            }
            contents.append(d)
        return contents
    except (KeyError, TypeError, ValueError):
        return None
