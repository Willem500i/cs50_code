-- Keep a log of any SQL queries you execute as you solve the mystery.

-- see how crime scene reports are organized
.schema crime_scene_reports
-- Gave:

-- CREATE TABLE crime_scene_reports (
-- id INTEGER,
-- month INTEGER,
-- day INTEGER,
-- street TEXT,
-- description TEXT,
-- PRIMARY KEY(id)
-- );

-- get a list of all crime scene reports to read
SELECT description FROM crime_scene_reports;

-- Too many reports to read, need search to be more specific, maybe add day and location?
SELECT description FROM crime_scene_reports
WHERE month = 7 AND day = 28
AND street = "Chamberlin Street";

-- Perfect! this gives us only one report, the one we want:

-- Theft of the CS50 duck took place at 10:15am at the Chamberlin Street courthouse.
-- Interviews were conducted today with three witnesses who were present at the time —
-- each of their interview transcripts mentions the courthouse.

-- We now know that it happened at 10:15 AM at the Chamberlin Street courthouse.
-- We also know there are three witnesses

-- lets see what data tables we have to use
.tables

-- interviews could be used to try to get more information.
SELECT transcript
FROM interviews
WHERE day=28 AND month=7 AND year=2020 AND transcript LIKE "%courthouse%";
-- This gives us our 3 interviews:
--1:
--Sometime within ten minutes of the theft, I saw the thief get into a car in the cour
--thouse parking lot and drive away. If you have security footage from the courthouse
--parking lot, you might want to look for cars that left the parking lot in that time
--frame.

-- WHAT WE KNOW: The car plate was spotted sometime within 10m of theft at courthouse.

--2:
--I don't know the thief's name, but it was someone I recognized.
--Earlier this morning, before I arrived at the courthouse,
--I was walking by the ATM on Fifer Street and
--saw the thief there withdrawing some money.

-- WHAT WE KNOW: Theif withdrew money on the ATM on Fifer Street on the morning of

--3:
--As the thief was leaving the courthouse, they called someone who talked to them
--for less than a minute. In the call, I heard the thief say that they were planni
--ng to take the earliest flight out of Fiftyville tomorrow. The thief then asked
--the person on the other end of the phone to purchase the flight ticket.

-- WHAT WE KNOW: Theif made a call around the time of the crime, they took the flight out
-- of fiftyville on the 29th, the call was less than a minute, and that the accomplice purchased the ticket

-- Lets look deeper into how security logs work
.schema courthouse_security_logs

-- We know that this table has id, year, month, day, hour, minute, activity, and liscence_plate.
-- We know the month, day, hour, and not exactly minute, because
-- We can go within 20 minutes to see which cars were at the courthouse

SELECT license_plate, minute FROM courthouse_security_logs
WHERE month = 7 AND day = 28 AND hour = 10 AND minute < 20;

-- Gives us a few possible liscence plates before and around 10:15:
-- R3G7486 at 8 mins
-- 13FNH73 at 14 mins
-- 5P2BI95 at 16 mins
-- 94KL13X at 18 mins
-- 6P58WS2 at 18 mins
-- 4328GD8 at 19 mins

-- We know one of our suspects is probably one of these, so now we can narrow the search
-- to find these peoples

.schema people -- gives us all the info of people, including their liscence plate. We
-- can use this along with the hour of the courthouse to find who was there.

SELECT name, license_plate FROM people
WHERE license_plate = "R3G7486" -- Brandon at 8 mins
OR license_plate = "13FNH73" -- Sophia at 14 mins
OR license_plate = "5P2BI95" -- Patrick at 16 mins
OR license_plate = "94KL13X" -- Ernest at 18 mins
OR license_plate = "6P58WS2" -- Amber at 18 mins
OR license_plate = "4328GD8"; -- Danielle at 19 mins

-- Now that we know one of these is the criminal, we can see which of them travelled
.schema passengers
-- gives us the info we need on each passenger: flight_id and passport_number
.schema flights
-- Tells us we can see the origin of each flight and where it went
.schema airports
-- Each airport has an id and a city

-- We can combine these to find which of our suspects left on a plane from fiftyville
SELECT city, id FROM airports; -- tells us that the fiftyville airport is id 8.

SELECT id, destination_airport_id FROM flights
WHERE origin_airport_id = 8 AND month = 7 AND day = 29;
-- this gives us 5 flight id's, 18,23,36,53. It also gives us the destination of each.

-- What we can combine to find who flew:
-- names with passport numbers
-- passport numbers with flight id
-- only flights going out on that date

SELECT people.name, flights.id FROM people
JOIN passengers ON passengers.passport_number=people.passport_number
JOIN flights ON flights.id=passengers.flight_id
WHERE flights.month = 7 AND flights.day = 29
AND (people.name = "Brandon" OR people.name = "Sophia" OR people.name = "Patrick" OR people.name = "Ernest" OR people.name = "Amber" OR people.name = "Danielle"
);

-- This gives us that Brandon travelled on flight id 23, Ernest on 36, Danielle on 36, and Sophia on 43
-- Lets get more info on these flights, nameley destination, date, time.

SELECT airports.city, flights.id FROM airports
JOIN flights ON flights.destination_airport_id=airports.id
WHERE flights.id = 23 OR flights.id = 36 OR flights.id = 43;

-- This gives us that Brandon took flight id 23 to San Fransisco,
-- Ernest and Danielle took flight 36 to London
-- Sophia took flight 43 to chicago

-- Now we have our three possible locations that the theif went: San Fransisco, London, or Chicago

SELECT flights.day, flights.hour, flights.minute, flights.id FROM flights
WHERE flights.id = 23 OR flights.id = 36 OR flights.id = 43;

-- We know the theif wanted the first plane out, and this gives us that the earliest plane from
-- our list is flight 36 to london at 8:20. This means our suspect went to London, and the theif
-- is either Ernest or Danielle.

-- To find this out, we just need to see which one accecced the ATM on Fifer Street on the morning
-- of the 28th.

.schema atm_transactions
-- Tells us with atm_location ("Fifer Street"), and transaction_type (withdraw) we can get the account number.
.schema bank_accounts
-- Tells us that with the account number we can get the person id,
.schema people
-- tells us we can get a persons name with their id

SELECT people.name FROM people
JOIN atm_transactions ON atm_transactions.account_number=bank_accounts.account_number
JOIN bank_accounts ON bank_accounts.person_id=people.id
WHERE atm_transactions.atm_location = "Fifer Street"
AND day = 28 AND month = 7
AND people.name = "Ernest" OR people.name = "Danielle"
AND transaction_type = "withdraw";

-- This outputs Ernest and Danielle, meaning both of them used this ATM on the day of the robbery.
-- We still need to rule out one of them

select id, phone_number from people where name = "Ernest";
-- Gets us Ernests phone number of (367) 555-5533

select phone_number from people where name = "Danielle";
-- Gets us Danielle's phone number of (389) 555-5198

.schema phone_calls
-- tells us how to get phone call info: caller and reviever on the day

select caller, duration, receiver from phone_calls where duration < 60 and day = 28
and (caller = "(389) 555-5198" or caller = "(367) 555-5533");

-- This gives us only one phone call, from (367) 555-5533 to (367) 555-5533 lasting 45 seconds.
-- lets find out who this call was to:

SELECT name, phone_number, license_plate FROM people WHERE phone_number = "(375) 555-8161";
-- This gives us one name: Berthold, our accomplice