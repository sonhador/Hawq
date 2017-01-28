DROP FUNCTION bytea_to_text_array(bytea);

--CREATE FUNCTION bytea_to_text_array(bytea) RETURNS text[] AS '$libdir/bytea_to_from_text_array.so', 'bytea_to_text_array' LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION bytea_to_text_array(bytea) RETURNS text[] AS $$ SELECT string_to_array(pg_catalog.encode($1, 'escape'), ',') $$ LANGUAGE SQL;

DROP FUNCTION text_array_to_bytea;

CREATE FUNCTION text_array_to_bytea(text[]) RETURNS bytea AS $$ SELECT pg_catalog.decode(array_to_string($1, ','), 'escape') $$ LANGUAGE SQL;

DROP CAST (bytea AS text[]);

CREATE CAST (bytea AS text[]) WITH FUNCTION bytea_to_text_array(bytea) AS ASSIGNMENT;

DROP CAST (text[] AS bytea);

CREATE CAST (text[] AS bytea) WITH FUNCTION text_array_to_bytea(text[]) AS ASSIGNMENT;

DROP TABLE src_tbl;

DROP TABLE dst_tbl;

CREATE TABLE src_tbl(col_src bytea) DISTRIBUTED RANDOMLY;

CREATE TABLE dst_tbl(col_dst text[]) DISTRIBUTED RANDOMLY;

TRUNCATE TABLE src_tbl;

INSERT INTO src_tbl

--SELECT '1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20'::bytea;

SELECT ARRAY[1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20]::text[];

TRUNCATE TABLE dst_tbl;

INSERT INTO dst_tbl

SELECT * FROM src_tbl;

SELECT * FROM dst_tbl;
