/*******************************************************************************
 * The MIT License (MIT)
 *
 * Copyright (c) 2017 MongJu Jung <mongju.jung@dell.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *******************************************************************************/

#include "postgres.h"
#include "utils/builtins.h"
#include "utils/array.h"
#include "fmgr.h"
#include <string.h>
#include <stdlib.h>

#ifdef PG_MODULE_MAGIC
PG_MODULE_MAGIC;
#endif

Datum bytea_to_text_array(PG_FUNCTION_ARGS);
void add_to_arr(char ***p_arr, int *max_arr_size, int *cur_arr_size, char *elem);
char *dup_pgtext(text *expr_text);

void add_to_arr(char ***p_arr, int *max_arr_size, int *cur_arr_size, char *elem) {
	char *copy;
	int len = strlen(elem);

	if (*cur_arr_size >= *max_arr_size) {
		*p_arr = (char **)realloc(*p_arr, *max_arr_size * 2 * sizeof(char *));	
		*max_arr_size = *max_arr_size * 2;
	}

	copy = (char *)malloc(len * sizeof(char) + 1);
	memcpy(copy, elem, len);
	copy[len] = 0;

	(*p_arr)[(*cur_arr_size)++] = copy;	
}

char *dup_pgtext(text *expr_text) {
	size_t len = VARSIZE(expr_text) - VARHDRSZ;
	char *dup = palloc(len + 1);

	memcpy(dup, VARDATA(expr_text), len);
	dup[len] = 0;
	
	return dup;
}

ArrayType *parse(text *expr_text) {
	char **arr;
	char *expr;
	char *elem;
	int max_arr_size = 1;
	int cur_arr_size = 0;
	int i;

	Datum *elems;
	ArrayType *result;
	int16 typlen;
	bool typbyval;
	char typalign;

	expr = dup_pgtext(expr_text);

	arr = (char **)malloc(max_arr_size * sizeof(char *));

	elem = strtok(expr, ",");
	while (elem != NULL) {
		add_to_arr(&arr, &max_arr_size, &cur_arr_size, elem);
		elem = strtok(NULL, ",");	
	}

	elems = (Datum *)palloc(cur_arr_size * sizeof(Datum));	

	for (i=0; i<cur_arr_size; i++) {
		elems[i] = PointerGetDatum(cstring_to_text(arr[i]));
		free(arr[i]);
	}
	
	free(arr);

	get_typlenbyvalalign(TEXTOID, &typlen, &typbyval, &typalign);
	result = construct_array(elems, cur_arr_size, TEXTOID, typlen, typbyval, typalign);

	return result;		
}

PG_FUNCTION_INFO_V1(bytea_to_text_array);
Datum bytea_to_text_array(PG_FUNCTION_ARGS) {
	PG_RETURN_ARRAYTYPE_P(parse(PG_GETARG_TEXT_P(0)));	
}
