-- list the titles of all movies in which both
-- Johnny Depp and Helena Bonham Carter starred.

SELECT movies.title FROM movies
JOIN stars ON stars.movie_id=movies.id
JOIN people ON stars.person_id=people.id
WHERE people.name = "Johnny Depp"
AND title in (
SELECT movies.title FROM movies
JOIN stars ON stars.movie_id=movies.id
JOIN people ON stars.person_id=people.id
WHERE people.name = "Helena Bonham Carter"
);