#!/bin/sh

rm -f *.o *.so

FLAGS="-O2 -m64 -march=native"

GPDB=/usr/local/greenplum-db
LIB=$GPDB/lib
INC=$GPDB/include
INC_POSTGRESQL=$INC/postgresql
INC_SERVER=$INC_POSTGRESQL/server
INC_INTERNAL=$INC_POSTGRESQL/internal

gcc $FLAGS -I$INC -I$INC_SERVER -I$INC_INTERNAL -L$LIB -fPIC -c bytea_to_from_text_array.c

gcc -shared -o bytea_to_from_text_array.so bytea_to_from_text_array.o

sudo cp bytea_to_from_text_array.so /usr/local/greenplum-db/lib/postgresql/.
