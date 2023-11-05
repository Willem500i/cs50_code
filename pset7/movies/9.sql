-- write a SQL query to list the names of all people who 
-- starred in a movie released in 2004, ordered by birth year

SELECT distinct name FROM people
JOIN movies ON movies.id=stars.movie_id
JOIN stars ON stars.person_id=people.id
WHERE movies.year = 2004
ORDER BY people.birth;