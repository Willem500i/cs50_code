-- list the names of all people who have directed a movie that received a rating of at least 9.0

SELECT distinct name FROM people
JOIN movies ON movies.id=directors.movie_id
JOIN directors ON directors.person_id=people.id
JOIN ratings ON movies.id=ratings.movie_id
WHERE ratings.rating >= 9.0;