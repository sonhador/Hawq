#!/bin/sh

rm -f Str2Map.jar

mkdir -p ./io/pivotal/kr/phd/pljava/str2map

javac -d . Parser.java

jar cvf Str2Map.jar io
