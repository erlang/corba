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

int ic_encode_tuple(char *buf, int *index, const ic_erlang_term *term);
int ic_encode_list(char *buf, int *index, const ic_erlang_term *term);

int ic_calculate_tuple_size(int *index, const ic_erlang_term *term);
int ic_calculate_list_size(int *index, const ic_erlang_term *term);

int ic_encode_term(char *buf, int *index, const ic_erlang_term *term)
{
   int retVal = 0;

   switch(term->type) {

   case ic_integer:
      retVal = ei_encode_long(buf, index, term->value.i_val);
      //fprintf(stdout, "Encoded ic_integer value: %ld\n", term->value.i_val);
     break;
   case ic_float:
      retVal = ei_encode_double(buf, index, term->value.d_val);
      //fprintf(stdout, "Encoded ic_float value: %f\n", term->value.d_val);
      break;
   case ic_atom:
      retVal = ei_encode_atom(buf, index, term->value.atom_name);
      //fprintf(stdout, "Encoded ic_atom value: %s\n", term->value.atom_name);
      break;
   case ic_pid:
      retVal = ei_encode_pid(buf, index, term->value.pid);
      //fprintf(stdout, "Encoded ic_pid\n");
      break;
   case ic_port:
      retVal = ei_encode_port(buf, index, term->value.port);
      //fprintf(stdout, "Encoded ic_port\n");
      break;
   case ic_ref:
      retVal = ei_encode_ref(buf, index, term->value.ref);
      //fprintf(stdout, "Encoded ic_ref\n");
      break;
   case ic_tuple:
      retVal = ic_encode_tuple(buf, index, term);
      break;
   case ic_list:
      retVal = ic_encode_list(buf, index, term);
      break;
   case ic_binary:
      retVal = ei_encode_binary(buf, index,
				term->value.bin->bytes,
				term->value.bin->size);
      //fprintf(stdout, "Encoded ic_binary\n");
      break;
   default:
      retVal = -1;
      break;
   }

   return retVal;
}

int ic_encode_tuple(char *buf, int *index, const ic_erlang_term *term)
{
   int retVal = 0;
   int arity, i;
   arity = term->value.tuple->arity;

   if((retVal = ei_encode_tuple_header(buf, index, arity)))
      goto error;
   //fprintf(stdout, "Encoded ic_tuple arity: %d\n", arity);

   for(i = 0; i < arity; i++)
      if((retVal = ic_encode_term(buf, index, term->value.tuple->elements[i])))
	 goto error;

 error:
   return retVal;
}

int ic_encode_list(char *buf, int *index, const ic_erlang_term *term)
{
   int retVal = 0;
   int arity, i;
   ic_erlang_list_elem *p = NULL;
   ic_erlang_term *elem;

   p = term->value.list->head;
   arity = term->value.list->arity;

   if(arity) {
      if((retVal = ei_encode_list_header(buf, index, arity)))
	    goto error;
      //fprintf(stdout, "Encoded ic_list arity: %d\n", arity);

      for(i = 0; i < arity; i++) {
	 elem = p->element;
	 if((retVal = ic_encode_term(buf, index, elem)))
	    goto error;
	 p = p->next;
      }
      if((retVal = ei_encode_empty_list(buf, index)))
	 goto error;
   } else
      retVal = ei_encode_empty_list(buf, index);

 error:
   return retVal;
}

int ic_size_of_encoded_term(int *index, const ic_erlang_term *term)
{
   int retVal = 0;

   switch(term->type) {

   case ic_integer:
      retVal = ei_encode_long(NULL, index, term->value.i_val);
      break;
   case ic_float:
      retVal = ei_encode_double(NULL, index, term->value.d_val);
      break;
   case ic_atom:
      retVal = ei_encode_atom(NULL, index, term->value.atom_name);
      break;
   case ic_pid:
      retVal = ei_encode_pid(NULL, index, term->value.pid);
      break;
   case ic_port:
      retVal = ei_encode_port(NULL, index, term->value.port);
      break;
   case ic_ref:
      retVal = ei_encode_ref(NULL, index, term->value.ref);
      break;
   case ic_tuple:
      retVal = ic_calculate_tuple_size(index, term);
      break;
   case ic_list:
      retVal = ic_calculate_list_size(index, term);
      break;
   case ic_binary:
      retVal = ei_encode_binary(NULL, index,
				term->value.bin->bytes,
				term->value.bin->size);
      break;
   default:
      retVal = -1;
      break;
   }

   return retVal;
}

int ic_calculate_tuple_size(int *index, const ic_erlang_term *term)
{
   int retVal = 0;
   int arity, i;
   arity = term->value.tuple->arity;

   if((retVal = ei_encode_tuple_header(NULL, index, arity)))
      goto error;

   for(i = 0; i < arity; i++)
      if((retVal = ic_size_of_encoded_term(index, term->value.tuple->elements[i])))
	 goto error;

 error:
   return retVal;
}

int ic_calculate_list_size(int *index, const ic_erlang_term *term)
{
   int retVal = 0;
   int arity, i;
   ic_erlang_list_elem *p = NULL;
   ic_erlang_term *elem;

   p = term->value.list->head;
   arity = term->value.list->arity;

   if(arity) {
      if((retVal = ei_encode_list_header(NULL, index, arity)))
	    goto error;

      for(i = 0; i < arity; i++) {
	 elem = p->element;
	 if((retVal = ic_size_of_encoded_term(index, elem)))
	    goto error;
	 p = p->next;
      }
      if((retVal = ei_encode_empty_list(NULL, index)))
	 goto error;
   } else
      retVal = ei_encode_empty_list(NULL, index);

 error:
   return retVal;
}
