# MemeGen50

A web application to view and save memes from your favorite subreddits

<hr>

#### Video Demo: https://youtu.be/kx9sRrUUbIw

#### Description

- The website asks users to login or register.
- After logging in, the user can select from the list of available subreddits and number of memes they want to see.
- When user submit the form above, the site will send a request to [meme-api](https://github.com/D3vd/Meme_Api)
- After getting response, it will show users the number of memes from the subreddit which user asked before.
- User can also save a meme by clicking on "Add to collections"
- User can remove a meme from their collections

#### Files

`app.py` : File where main program exist. Here we configure the app and session. We configured the database using CS50's SQL module and will use sqlite3 database.
Created two tables, "users" to store username and password of users and "collections" to store memes that users save.
Also, there are number of routes defined in this file

- `"/"`: Main page of the website. Renders `index.html`.
- `"/register"`: To register a user. We can reach here by using both GET and POST methods. When reached by GET, renders `register.html` which shows user a form to register.
  When reached via POST, validate form responses and add user to the database.
- `"/login"`: To log in a user. When reached via GET request, renders `login.html` which shows user a form to login. When reached via POSt, validate form responses and log in the user
- `"/logout"`: Log out the user
- `"/memegen"`: Accepts form response from user consisting name of subreddit and number of memes. After validating the responses calls function `get_memes` which is defined in `helpers.py`. This function return a dictionary containing name of subreddit, title of meme and image link of the meme. Renders `memes.html` which displays memes on the site.
- `"/add"`: Add a meme to the user's collection.
- `"/show"`: Renders `show.html` which displays memes present in the user's collections.
- `"/remove/<int:id>"`: Removes a meme from the user's collection. Takes id of the meme to be removed as argument.

<br>
<br>

`helpers.py`: This file contains a number of function definitions we need in our main file

- `apology`: Replaces the special character and then renders `apology.html`. This function is called whenever there is a bad request by the user.
- `get_memes`: This function send request to the meme-api and get response. After validating the response, returns a list of dictionaries containg information related to all the memes.

#### Running

Start flask's built-in webserver by executing following command inside `/memegen50`. <br>

```bash
$ flask run
```
