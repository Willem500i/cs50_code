-- list the names of all people who starred in Toy Story

SELECT name FROM people
JOIN movies on movies.id=stars.movie_id
JOIN stars on people.id=stars.person_id
WHERE movies.title = "Toy Story";