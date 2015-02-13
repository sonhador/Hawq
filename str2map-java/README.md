Str2Map - PL/Java Procedure
=========

Str2Map is a PostgreSQL PL/Java Procedure that lets you parse and get values for map-styled string expression.

Example Usage
----
CREATE OR REPLACE FUNCTION parse(body text, key text, keyValPairsDelim text, keyValDelim text)</br>
RETURNS TEXT</br>
  AS 'io.pivotal.kr.phd.pljava.str2map.Parser.extract(java.lang.String,java.lang.String,java.lang.String,java.lang.String)'</br>
LANGUAGE pljava;</br>

DROP TABLE IF EXISTS test_t;<br/>
CREATE TABLE test_t (<br/>
body text<br/>
);<br/>
INSERT INTO test_t VALUES<br/>
('k1:v1,k2:,k3:'),<br/>
('k1:,k2:v2,k3:'),<br/>
('k1:va1,k2:val2,k3:val3'),<br/>
('kkk:k1,kk:k2,kkkk:k3');<br/>

SELECT parse(body, 'k1', ',', ':') as k1,<br/>
       parse(body, 'k2', ',', ':') as k2,<br/>
       parse(body, 'k3', ',', ':') as k3,<br/>
       parse(body, 'k4', ',', ':') as k4,<br/>
       parse(body, 'kk', ',', ':') as kk<br/>
FROM test_t;

Build & Deploy & Register
----
1. run, ./compile.sh to build Str2Map.jar
2. place Str2Map.jar under $GPHOME/lib/postgresql/java
3. run, cd $GPHOME/lib/postgresql/java gpscp -f ~/hosts.txt Str2Map.jar =:$GPHOME/lib/postgresql/java/
   where ~/hosts.txt contains all the Hawq nodes
4. run, gpconfig -c pljava_classpath -v \’Str2Map.jar\’  
5. run, gpstop -u

Version
----
1.0

Credit
----
"MongJu Jung" <mjung@pivotal.io> 
