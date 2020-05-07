/*
 * %CopyrightBegin%
 *
 * Copyright Ericsson AB 1998-2020. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * %CopyrightEnd%
 */
#include "ic.h"

int ic_decode_tuple(const char *buf, int *index, ic_erlang_term *term);
int ic_decode_string(const char *buf, int *index, int length, ic_erlang_term *term);
int ic_decode_list(const char *buf, int *index, ic_erlang_term *term);
ic_erlang_term* ic_mk_empty_term(ic_erlang_type type);

int ic_decode_term(const char *buf, int *index, ic_erlang_term **term)
{
   int retVal = 0;
   ic_erlang_type ictype;
   int size;
   long tmpsize;
   ic_erlang_term *tmp_term = NULL;
   char *atom;
   ic_erlang_binary *bin;

   /* fprintf(stderr, "DECODE TERM\n"); */
   if(ic_get_type(buf, index, &ictype, &size))
      return -1;
   /* fprintf(stderr, "Type: %d\n", ictype); */

   if(term) {
      switch(ictype) {

      case ic_integer:
	 tmp_term = ic_mk_empty_term(ictype);
	 if(tmp_term) {
	    retVal = ei_decode_long(buf, index, &(tmp_term->value.i_val));
	    /* fprintf(stderr, "Decoded ic_integer value: %ld\n", tmp_term->value.i_val); */
	 } else
	    retVal = -1;
	 break;
      case ic_float:
	 tmp_term = ic_mk_empty_term(ictype);
	 if(tmp_term) {
	    retVal = ei_decode_double(buf, index, &(tmp_term->value.d_val));
	    /* fprintf(stderr, "Decoded ic_float value: %f\n", tmp_term->value.d_val); */
	 } else
	    retVal = -1;
	 break;
      case ic_atom:
	 atom = (char*) malloc(size+1);
	 if(atom) {
	    tmp_term = ic_mk_empty_term(ictype);
	    if(tmp_term) {
	       tmp_term->value.atom_name = atom;
	       retVal = ei_decode_atom(buf, index, atom);
	       /* fprintf(stderr, "Decoded ic_atom value: %s\n", tmp_term->value.atom_name); */
	    } else {
	       retVal = -1;
	       CORBA_free(atom);
	    }
	 } else
	    retVal = -1;
	 break;
      case ic_pid:
	 tmp_term = ic_mk_empty_term(ictype);
	 if(tmp_term) {
	    tmp_term->value.pid = (erlang_pid*) malloc(sizeof(erlang_pid));
	    if(tmp_term->value.pid) {
	       retVal = ei_decode_pid(buf, index, tmp_term->value.pid);
	       /* fprintf(stderr, "Decoded ic_pid\n"); */
	    } else {
	       CORBA_free(tmp_term);
	       retVal = -1;
	    }
	 } else
	    retVal = -1;
	 break;
      case ic_port:
	 tmp_term = ic_mk_empty_term(ictype);
	 if(tmp_term) {
	    tmp_term->value.port = (erlang_port*) malloc(sizeof(erlang_port));
	    if(tmp_term->value.port) {
	       retVal = ei_decode_port(buf, index,  tmp_term->value.port);
	       /* fprintf(stderr, "Decoded ic_port\n"); */
	    } else {
	       CORBA_free(tmp_term);
	       retVal = -1;
	    }
	 } else
	    retVal = -1;
	 break;
      case ic_ref:
	 tmp_term = ic_mk_empty_term(ictype);
	 if(tmp_term) {
	    tmp_term->value.ref = (erlang_ref*) malloc(sizeof(erlang_ref));
	    if(tmp_term->value.ref) {
	       retVal = ei_decode_ref(buf, index, tmp_term->value.ref);
	       /* fprintf(stderr, "Decoded ic_ref\n"); */
	    } else {
	       CORBA_free(tmp_term);
	       retVal = -1;
	    }
	 } else
	    retVal = -1;
	 break;
      case ic_tuple:
	 tmp_term = ic_mk_tuple_term(size);
	 if(tmp_term) {
	    retVal = ic_decode_tuple(buf, index, tmp_term);
	 } else
	    retVal = -1;
	 break;
      case ic_string:
	 /* Represented by a list as in old implementation */
	 tmp_term = ic_mk_list_term();
	 if(tmp_term) {
	    retVal = ic_decode_string(buf, index, size, tmp_term);
	    /* fprintf(stderr, "Start list\n"); */
	    /* ic_print_erlang_term(tmp_term); */
	    /* fprintf(stderr, "End list\n"); */
	 } else
	    retVal = -1;
	 break;
      case ic_list:
	 tmp_term = ic_mk_list_term();
	 if(tmp_term) {
	    retVal = ic_decode_list(buf, index, tmp_term);
	 } else
	    retVal = -1;
	 break;
      case ic_binary:
	 /* NOTICE: */
	 /* ic_get_type() returns length as int and ei_decode_binary() */
	 /* returns length as long so the allocated buffer could be to small */
	 bin = (ic_erlang_binary*) malloc(sizeof(ic_erlang_binary));
	 if(bin) {
	    tmp_term = ic_mk_empty_term(ictype);
	    if(tmp_term) {
	       tmp_term->value.bin = bin;
	       bin->size = size;
	       bin->bytes = (char*) malloc(size);
	       if(bin->bytes) {
		  retVal = ei_decode_binary(buf, index, bin->bytes, &tmpsize);
		  /* fprintf(stderr, "Decoded ic_binary\n"); */
	       } else
		  retVal = -1;
	    } else {
	       retVal = -1;
	       CORBA_free(bin);
	    }
	 } else
	    retVal = -1;
	 break;
      }

      if(retVal) {
	 ic_free_erlang_term(tmp_term);
	 *term = NULL;
      } else
	 *term = tmp_term;

   } else
      switch(ictype) {

      case ic_integer:
	 retVal = ei_decode_long(buf, index, NULL);
	 break;
      case ic_float:
	 retVal = ei_decode_double(buf, index, NULL);
	 break;
      case ic_atom:
	 retVal = ei_decode_atom(buf, index, NULL);
	 break;
      case ic_pid:
	 retVal = ei_decode_pid(buf, index, NULL);
	 break;
      case ic_port:
	 retVal = ei_decode_port(buf, index, NULL);
	 break;
      case ic_ref:
	 retVal = ei_decode_ref(buf, index, NULL);
	 break;
      case ic_tuple:
	 retVal = ic_decode_tuple(buf, index, NULL);
	 break;
      case ic_string:
	 retVal = ic_decode_string(buf, index, size, NULL);
	 break;
      case ic_list:
	 retVal = ic_decode_list(buf, index, NULL);
	 break;
      case ic_binary:
	 retVal = ei_decode_binary(buf, index, NULL, &tmpsize);
	 break;
      }

   return retVal;
}

ic_erlang_term* ic_mk_empty_term(ic_erlang_type type)
{
   ic_erlang_term *term = NULL;

   term = (ic_erlang_term *) malloc(sizeof(ic_erlang_term));
   if(term)
      term->type = type;

   return term;
}

int ic_decode_tuple(const char *buf, int *index, ic_erlang_term *tuple)
{
   int retVal = 0;
   int arity, i;
   ic_erlang_term *tmp_term;

   /* The calling function delete tuple if an error is returned */
   if((retVal = ei_decode_tuple_header(buf, index, &arity)))
      goto error;

   if(arity) {
      /* fprintf(stderr, "Decoded ic_tuple arity: %d\n", arity); */
      if(tuple)
	 for(i = 0; i < arity; i++) {
	    retVal = ic_decode_term(buf, index, &tmp_term);
	    if(retVal) goto error;
	    ic_tuple_add_elem(tuple, tmp_term, i);
	 }
      else
	 for(i = 0; i < arity; i++) {
	    retVal = ic_decode_term(buf, index, NULL);
	    if(retVal) goto error;
	 }
   }

 error:
   return retVal;
}

int ic_decode_string(const char *buf, int *index, int length, ic_erlang_term *list)
{
   int retVal = 0;
   int i;
   ic_erlang_term *tmp_term;
   char *str = NULL;

   /* The calling function delete list if an error is returned */
   if(list) {
      /* fprintf(stderr, "String length: %d\n", length); */
      str = (char*) malloc(length+1);
      if(str) {
	 retVal = ei_decode_string(buf, index, str);
	 if(retVal) goto error;

	 /* fprintf(stderr, "Decoded ic_string: %s\n", str); */
	 for(i = 0; i < length; i++) {
	    tmp_term = ic_mk_int_term((long) str[i]);
	    /* fprintf(stderr, "str[%d]: %d\n", i, str[i]); */
	    if(ic_list_add_elem(list, tmp_term)) {
	       retVal = -1;
	       goto error;
	    }
	 }
	 /* fprintf(stderr, "List Finished\n"); */
	 /* ic_print_erlang_term(list); */
      } else
	 retVal = -1;
   } else
      retVal = ei_decode_string(buf, index, NULL);

 error:
   CORBA_free(str);
   return retVal;
}

int ic_decode_list(const char *buf, int *index, ic_erlang_term *list)
{
   int retVal = 0;
   int arity, zero_arity, i;
   ic_erlang_term *tmp_term = NULL;

   /* The calling function delete list if an error is returned */
   retVal = ei_decode_list_header(buf, index, &arity);
   if(retVal) goto error;

   /* fprintf(stderr, "Decoded ic_list arity: %d\n", arity); */
   if(arity) {
      if(list)
	 for(i = 0; i < arity; i++) {
	    retVal = ic_decode_term(buf, index, &tmp_term);
	    if(retVal) {
	       retVal = -1;
	       goto error;
	    }
	    if(ic_list_add_elem(list, tmp_term)) {
	       retVal = -1;
	       goto error;
	    }
	 }
      else
	 for(i = 0; i < arity; i++) {
	    retVal = ic_decode_term(buf, index, NULL);
	    if(retVal) {
	       retVal = -1;
	       goto error;
	    }
	 }

      retVal = ei_decode_list_header(buf, index, &zero_arity);
      if(retVal && (zero_arity != 0)) {
	 /* fprintf(stderr, "Not empty list: %d\n", zero_arity); */
	 goto error;
      }
   }

 error:
   return retVal;
}
