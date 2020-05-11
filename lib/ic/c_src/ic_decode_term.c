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

int ic_decode_tuple(const char *buf, int *index, char *outbuf, int *pos);
int ic_decode_string(const char *buf, int *index, int length, char *outbuf, int *pos);
int ic_decode_list(const char *buf, int *index, char *outbuf, int *pos);
/* ic_erlang_term* ic_mk_empty_term(ic_erlang_type type); */

int ic_calculate_decoded_tuple_size(const char *buf, int *index, int *size);
int ic_calculate_decoded_list_size(const char *buf, int *index, int *size);
int ic_decode_term_into_memory(const char *buf, int *index, char *outbuf, int *pos);

/* Size calculation */
int ic_size_of_decoded_term(const char *buf, int *index, int *size)
{
   int retVal = 0,
      type_size,
      i;
   long tmpsize;
   ic_erlang_type ictype;

   *size = OE_ALIGN(*size + sizeof(ic_erlang_term));

   if(ic_get_type(buf, index, &ictype, &type_size))
      return -1;

   switch(ictype) {
   case ic_integer:
      retVal = ei_decode_long(buf, index, NULL);
      break;
   case ic_float:
      retVal = ei_decode_double(buf, index, NULL);
      break;
   case ic_atom:
      *size = OE_ALIGN(*size + type_size + 1);
      retVal = ei_decode_atom(buf, index, NULL);
      break;
   case ic_pid:
      *size = OE_ALIGN(*size + sizeof(erlang_pid));
      retVal = ei_decode_pid(buf, index, NULL);
      break;
   case ic_port:
      *size = OE_ALIGN(*size + sizeof(erlang_port));
      retVal = ei_decode_port(buf, index, NULL);
      break;
   case ic_ref:
      *size = OE_ALIGN(*size + sizeof(erlang_ref));
      retVal = ei_decode_ref(buf, index, NULL);
      break;
   case ic_tuple:
      retVal = ic_calculate_decoded_tuple_size(buf, index, size);
      break;
   case ic_string:
      *size = OE_ALIGN(*size + sizeof(ic_erlang_list));
      for(i = 0; i < type_size; i++) {
	 *size = OE_ALIGN(*size + sizeof(ic_erlang_list_elem));
	 *size = OE_ALIGN(*size + sizeof(ic_erlang_term));
      }
      retVal = ei_decode_string(buf, index, NULL);
      break;
   case ic_list:
      retVal = ic_calculate_decoded_list_size(buf, index, size);
      break;
   case ic_binary:
      *size = OE_ALIGN(*size + sizeof(ic_erlang_binary));
      *size = OE_ALIGN(*size + type_size);
      retVal = ei_decode_binary(buf, index, NULL, &tmpsize);
      break;
   }
   return retVal;
}

int ic_calculate_decoded_tuple_size(const char *buf, int *index, int *size)
{
   int retVal = 0;
   int arity, i;

   if((retVal = ei_decode_tuple_header(buf, index, &arity)))
      goto error;

   *size = OE_ALIGN(*size + sizeof(ic_erlang_tuple));
   if(arity) {
      *size = OE_ALIGN(*size + sizeof(ic_erlang_term*) * arity);
      for(i = 0; i < arity; i++) {
	 if((retVal = ic_size_of_decoded_term(buf, index, size)))
	    goto error;
      }
   }

 error:
   return retVal;
}

int ic_calculate_decoded_list_size(const char *buf, int *index, int *size)
{
   int retVal = 0;
   int i, arity, zero_arity;

   if((retVal = ei_decode_list_header(buf, index, &arity)))
      goto error;

   *size = OE_ALIGN(*size + sizeof(ic_erlang_list));
   if(arity) {
      for(i = 0; i < arity; i++) {
	 *size = OE_ALIGN(*size + sizeof(ic_erlang_list_elem));
	 if((retVal = ic_size_of_decoded_term(buf, index, size)))
	    goto error;
      }

      retVal = ei_decode_list_header(buf, index, &zero_arity);
      if(retVal && (zero_arity != 0)) {
	 fprintf(stderr, "Not empty list: %d\n", zero_arity);
	 goto error;
      }
   }

 error:
   return retVal;
}

/* Decoding */
int ic_decode_term(const char *buf, int *index, ic_erlang_term **term)
{
   int retVal = 0,
      size_index = *index,
      size = 0,
      pos = 0;
   char *outbuf;

   if(term) {
      if(!ic_size_of_decoded_term(buf, &size_index, &size)) {
	 outbuf = (char *) malloc(size);
	 if(outbuf) {
	    ((ic_erlang_term *) outbuf)->_one_block_alloc = 1;
	    if(ic_decode_term_into_memory(buf, index, outbuf, &pos)) {
	       CORBA_free(outbuf);
	       *term = NULL;
	       retVal = -1;
	    } else
	       *term = (ic_erlang_term *) outbuf;
	 } else
	    retVal = -1;
      } else
	 retVal = -1;
  } else
      retVal = ic_decode_term_into_memory(buf, index, NULL,  &pos);

   return retVal;
}

int ic_decode_term_into_memory(const char *buf, int *index, char *outbuf, int *pos)
{
   int retVal = 0;
   ic_erlang_type ictype;
   int size;
   long tmpsize;
   /* char *atom; */
   /* ic_erlang_binary *bin; */
   char *write_pos, *tmp_pos;

   fprintf(stderr, "DECODE TERM\n");
   if(ic_get_type(buf, index, &ictype, &size))
      return -1;
   fprintf(stderr, "Type: %d\n", ictype);

   if(outbuf) {
      write_pos = outbuf + *pos;
      switch(ictype) {
      case ic_integer:
	 ((ic_erlang_term *) write_pos)->type = ic_integer;
	 retVal = ei_decode_long(buf, index, &(((ic_erlang_term *) write_pos)->value.i_val));
	 if(!retVal) {
	    *pos = OE_ALIGN(*pos + sizeof(ic_erlang_term));
	    fprintf(stderr, "Decoded ic_integer value: %ld\n", ((ic_erlang_term *) write_pos)->value.i_val);
	 }
	 break;
      case ic_float:
	 ((ic_erlang_term *) write_pos)->type = ic_float;
	 retVal = ei_decode_double(buf, index, &(((ic_erlang_term *) write_pos)->value.d_val));
	 if(!retVal) {
	    *pos = OE_ALIGN(*pos + sizeof(ic_erlang_term));
	    fprintf(stderr, "Decoded ic_float value: %f\n", ((ic_erlang_term *) write_pos)->value.d_val);
	 }
	 break;
      case ic_atom:
	 if(size < MAXATOMLEN_UTF8) {
	    ((ic_erlang_term *) write_pos)->type = ic_atom;
	    *pos = OE_ALIGN(*pos + sizeof(ic_erlang_term));
	    ((ic_erlang_term *) write_pos)->value.atom_name = (char *) (outbuf + *pos);
	    retVal = ei_decode_atom(buf, index, ((ic_erlang_term *) write_pos)->value.atom_name);
	    if(!retVal) {
	       *pos = OE_ALIGN(*pos + size + 1);
	       fprintf(stderr, "Decoded ic_atom: %s\n", ((ic_erlang_term *) write_pos)->value.atom_name);
	    } else
	       retVal = -1;
	 } else
	    retVal = -1;
	 break;
      case ic_pid:
	 ((ic_erlang_term *) write_pos)->type = ic_pid;
	 *pos = OE_ALIGN(*pos + sizeof(ic_erlang_term));
	 ((ic_erlang_term *) write_pos)->value.pid = (erlang_pid *) (outbuf + *pos);
	 retVal = ei_decode_pid(buf, index, ((ic_erlang_term *) write_pos)->value.pid);
	 if(!retVal) {
	    *pos = OE_ALIGN(*pos + sizeof(erlang_pid));
	    fprintf(stderr, "Decoded ic_pid\n");
	 } else
	    retVal = -1;
	 break;
      case ic_port:
	 ((ic_erlang_term *) write_pos)->type = ic_port;
	 *pos = OE_ALIGN(*pos + sizeof(ic_erlang_term));
	 ((ic_erlang_term *) write_pos)->value.port = (erlang_port *) (outbuf + *pos);
	 retVal = ei_decode_port(buf, index, ((ic_erlang_term *) write_pos)->value.port);
	 if(!retVal) {
	    *pos = OE_ALIGN(*pos + sizeof(erlang_port));
	    fprintf(stderr, "Decoded ic_port\n");
	 } else
	    retVal = -1;
	 break;
      case ic_ref:
	 ((ic_erlang_term *) write_pos)->type = ic_ref;
	 *pos = OE_ALIGN(*pos + sizeof(ic_erlang_term));
	 ((ic_erlang_term *) write_pos)->value.ref = (erlang_ref *) (outbuf + *pos);
	 retVal = ei_decode_ref(buf, index, ((ic_erlang_term *) write_pos)->value.ref);
	 if(!retVal) {
	    *pos = OE_ALIGN(*pos + sizeof(erlang_ref));
	    fprintf(stderr, "Decoded ic_ref\n");
	 } else
	    retVal = -1;
	 break;
      case ic_tuple:
	 retVal = ic_decode_tuple(buf, index, outbuf, pos);
	 break;
      case ic_string:
	 /* Represented by a list as in old implementation */
	 retVal = ic_decode_string(buf, index, size, outbuf, pos);
	 break;
      case ic_list:
	 retVal = ic_decode_list(buf, index,  outbuf, pos);
	 break;
      case ic_binary:
	 /* NOTICE: */
	 /* ic_get_type() returns length as int and ei_decode_binary() */
	 /* returns length as long so the allocated buffer could be to small */
	 ((ic_erlang_term *) write_pos)->type = ic_binary;
	 *pos = OE_ALIGN(*pos + sizeof(ic_erlang_term));
	 tmp_pos = outbuf + *pos;
	 ((ic_erlang_term *) write_pos)->value.bin = (ic_erlang_binary *) tmp_pos;
	 write_pos = tmp_pos;
	 ((ic_erlang_binary *) write_pos)->size = size;
	 *pos = OE_ALIGN(*pos + sizeof(ic_erlang_binary));
	 ((ic_erlang_binary *) write_pos)->bytes = (char *) (outbuf + *pos);
	 retVal = ei_decode_binary(buf, index, ((ic_erlang_binary *) write_pos)->bytes, &tmpsize);
	 if(!retVal) {
	    *pos = OE_ALIGN(*pos + size);
	    fprintf(stderr, "Decoded ic_binary\n");
	 } else
	    retVal = -1;
	 break;
      }

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
	 retVal = ic_decode_tuple(buf, index, NULL, pos);
	 break;
      case ic_string:
	 retVal = ic_decode_string(buf, index, size, NULL, pos);
	 break;
      case ic_list:
	 retVal = ic_decode_list(buf, index, NULL, pos);
	 break;
      case ic_binary:
	 retVal = ei_decode_binary(buf, index, NULL, &tmpsize);
	 break;
      }
   return retVal;
}

/* ic_erlang_term* ic_mk_empty_term(ic_erlang_type type) */
/* { */
/*    ic_erlang_term *term = NULL; */

/*    term = (ic_erlang_term *) malloc(sizeof(ic_erlang_term)); */
/*    if(term) */
/*       term->type = type; */

/*    return term; */
/* } */

int ic_decode_tuple(const char *buf, int *index, char *outbuf, int *pos)
{
   int retVal = 0;
   int arity, i;
   char *write_pos, *tmp_pos;

   /* The calling function delete tuple if an error is returned */
   if((retVal = ei_decode_tuple_header(buf, index, &arity)))
      goto error;

   fprintf(stderr, "Decoded ic_tuple arity: %d\n", arity);

   if(outbuf) {
      write_pos = outbuf + *pos;
      ((ic_erlang_term *) write_pos)->type = ic_tuple;
      *pos = OE_ALIGN(*pos + sizeof(ic_erlang_term));
      tmp_pos = outbuf + *pos;
      ((ic_erlang_term *) write_pos)->value.tuple = (ic_erlang_tuple *) tmp_pos;
      write_pos = tmp_pos;

      ((ic_erlang_tuple *) write_pos)->arity = arity;
      *pos = OE_ALIGN(*pos + sizeof(ic_erlang_tuple));
      ((ic_erlang_tuple *) write_pos)->elements = (ic_erlang_term **) (outbuf + *pos);
      *pos = OE_ALIGN(*pos + sizeof(ic_erlang_term*) * arity);

      for(i = 0; i < arity; i++) {
	 ((ic_erlang_tuple *) write_pos)->elements[i] = (ic_erlang_term *) (outbuf + *pos);
	 retVal = ic_decode_term_into_memory(buf, index, outbuf, pos);
	 if(retVal) goto error;
      }
   } else
      for(i = 0; i < arity; i++) {
	 retVal = ic_decode_term_into_memory(buf, index, NULL, pos);
	 if(retVal) goto error;
      }

 error:
   return retVal;
}

int ic_decode_string(const char *buf, int *index, int length, char *outbuf, int *pos)
{
   int retVal = 0;
   int i;
   char *str = NULL;
   char *write_pos, *elem_pos, *tmp_pos;

   /* The calling function delete list if an error is returned */
   if(outbuf) {
      write_pos = outbuf + *pos;
      ((ic_erlang_term *) write_pos)->type = ic_list;
      *pos = OE_ALIGN(*pos + sizeof(ic_erlang_term));
      tmp_pos = outbuf + *pos;
      ((ic_erlang_term *) write_pos)->value.list = (ic_erlang_list *) tmp_pos;
      write_pos = tmp_pos;

      fprintf(stderr, "String length: %d\n", length);
      ((ic_erlang_list *) write_pos)->arity = length;
      ((ic_erlang_list *) write_pos)->head = NULL;
      ((ic_erlang_list *) write_pos)->tail = NULL;
      *pos = OE_ALIGN(*pos + sizeof(ic_erlang_list));

      str = (char*) malloc(length+1);

      if(str) {
	 retVal = ei_decode_string(buf, index, str);
	 if(retVal) goto error;
	 fprintf(stderr, "Decoded ic_string: %s\n", str);

	 for(i = 0; i < length; i++) {
	    elem_pos = outbuf + *pos;
	    *pos = OE_ALIGN(*pos + sizeof(ic_erlang_list_elem));

	    tmp_pos = outbuf + *pos;
	    ((ic_erlang_list_elem *) elem_pos)->element = (ic_erlang_term *) tmp_pos;
	    ((ic_erlang_term *) tmp_pos)->type = ic_integer;
	    ((ic_erlang_term *) tmp_pos)->value.i_val = str[i];
	    *pos = OE_ALIGN(*pos + sizeof(ic_erlang_term));
	    ((ic_erlang_list_elem *) elem_pos)->next = NULL;

	    if(((ic_erlang_list *) write_pos)->head == NULL) {
	       ((ic_erlang_list *) write_pos)->head = (ic_erlang_list_elem *) elem_pos;
	       ((ic_erlang_list *) write_pos)->tail = (ic_erlang_list_elem *) elem_pos;
	    } else  {
	       ((ic_erlang_list *) write_pos)->tail->next = (ic_erlang_list_elem *) elem_pos;
	       ((ic_erlang_list *) write_pos)->tail = (ic_erlang_list_elem *) elem_pos;
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

int ic_decode_list(const char *buf, int *index, char *outbuf, int *pos)
{
   int retVal = 0;
   int arity, zero_arity, i;
   char *write_pos, *elem_pos, *tmp_pos;

   /* The calling function delete list if an error is returned */
   retVal = ei_decode_list_header(buf, index, &arity);
   if(retVal) goto error;

   if(outbuf) {
      write_pos = outbuf + *pos;
      ((ic_erlang_term *) write_pos)->type = ic_list;
      *pos = OE_ALIGN(*pos + sizeof(ic_erlang_term));
      tmp_pos = outbuf + *pos;
      ((ic_erlang_term *) write_pos)->value.list = (ic_erlang_list *) tmp_pos;
      write_pos = tmp_pos;

      fprintf(stderr, "Decoded ic_list arity: %d\n", arity);
      ((ic_erlang_list *) write_pos)->arity = arity;
      ((ic_erlang_list *) write_pos)->head = NULL;
      ((ic_erlang_list *) write_pos)->tail = NULL;
      *pos = OE_ALIGN(*pos + sizeof(ic_erlang_list));

      for(i = 0; i < arity; i++) {
	 elem_pos = outbuf + *pos;
	 *pos = OE_ALIGN(*pos + sizeof(ic_erlang_list_elem));

	 tmp_pos = outbuf + *pos;
	 ((ic_erlang_list_elem *) elem_pos)->element = (ic_erlang_term *) tmp_pos;
	 ((ic_erlang_list_elem *) elem_pos)->next = NULL;

	 retVal = ic_decode_term_into_memory(buf, index, outbuf, pos);
	 if(retVal) {
	    retVal = -1;
	    goto error;
	 }

	 if(((ic_erlang_list *) write_pos)->head == NULL) {
	    ((ic_erlang_list *) write_pos)->head = (ic_erlang_list_elem *) elem_pos;
	    ((ic_erlang_list *) write_pos)->tail = (ic_erlang_list_elem *) elem_pos;
	 } else  {
	    ((ic_erlang_list *) write_pos)->tail->next = (ic_erlang_list_elem *) elem_pos;
	    ((ic_erlang_list *) write_pos)->tail = (ic_erlang_list_elem *) elem_pos;
	 }
      }

      if(arity) {
	 retVal = ei_decode_list_header(buf, index, &zero_arity);
	 if(retVal && (zero_arity != 0)) {
	    /* fprintf(stderr, "Not empty list: %d\n", zero_arity); */
	    goto error;
	 }
      }
   } else
      for(i = 0; i < arity; i++) {

	 retVal = ic_decode_term(buf, index, NULL);
	 if(retVal) {
	    retVal = -1;
	    goto error;
	 }
      }

 error:
   return retVal;
}
