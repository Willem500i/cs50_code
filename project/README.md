# Card Calculator
#### Video Demo:  https://www.youtube.com/watch?v=mBg1EkqbLqg
#### Description:
Using the cs50 finance website from pset 9 as a base, I have created a website to  take as inputs from users graphics card overclocks and hashrates for mining ethereum and other cryptocurrencies, then outputting their profitability based on current rates, including options to put in custom numbers. To create an accurate idea of the profitability of certain cards, the inputs are averaged into one, semi-accurate number for each card, and preventing spam or bad numbers by checking for a percent difference below 30% before adding the new values.

##### layout.html
My layout page is based off of the finance page, but I have changed it to be more appropriate for my site. At the bottom I credit my profitability data to whatttomine, the api used to find current crypto profitability rates. At the top it has a link to the homepage, as well as links to navigate to report.html and calculate.html to either report values or calculate profitability.

##### index.html
The homepage, index.html, is simply a picture and gif of a graphics card, as well as crediting myself with creating and managing the site.

##### report.html
The report page has a dropdown menu going through all the cards on the site to add a value to, with a survey to input card information values. In application.py, this information is then stored into two sql tables in cards.db. The first is simply a list of all entries, named responses, which stores an id, core, mem, pow, model, hash, card, and miner for every entry. The information is also averaged into the averages sql table, first cheching that the new values are less than 30% different from the current ones, and then averaging the new entries into the average for the specific card being reported.

##### calculate.html
The calculate page allows for users to calculate the profitability of any card in our database from a dropdown, as well as inputing electricity costs and hardware cost to calculate breakeven. It also allows for a custom card option, to put in hashrates of cards not in the site's database. After this information is put in and calculate is clicked, a api request is sent to whattomine.com to find the current Ethereum profitability numbers for the given hashrate.

##### application.py
This python code is what runs all of the functionality of the site, using simple get, post, and sql methods that I learned mostly from finance.

##### cards.db
This database file has my two storage locations for card information, a table with average card values and a table with every response ever given to the site, sorted by id. In order to add a new card to the list of cards I need to manually add a new entry in average, this is to prevent people from making a lot of cards that overlap with eachother and clutter the selection list.