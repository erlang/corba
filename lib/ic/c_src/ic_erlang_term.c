/*
 * %CopyrightBegin%
 *
 * Copyright Ericsson AB 2020-2021. All Rights Reserved.
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
 * See the License for the specific language governing peric_mk_int_termmissions and
 * limitations under the License.
 *
 * %CopyrightEnd%
 */
#include "ic.h"

void ic_free_tuple(ic_erlang_tuple *tuple);
void ic_free_list(ic_erlang_list *list);
void ic_print_tuple(ic_erlang_tuple *tuple);
void ic_print_list(ic_erlang_list *list);
int ic_compare_list(ic_erlang_list *list1, ic_erlang_list *list2);

/* Compare function */
int ic_erlang_term_is_equal(ic_erlang_term *t1, ic_erlang_term *t2) {
   int retVal = 1;
   int i;

   switch(t1->type) {
   case ic_integer:
      retVal = t2->type == ic_integer &&
	 t1->value.i_val == t2->value.i_val;
      break;
   case ic_float:
      retVal = t2->type == ic_float &&
	 t1->value.d_val == t2->value.d_val;
      break;
   case ic_atom:
      retVal = t2->type == ic_atom &&
	 !strcmp(t1->value.atom_name,
		 t2->value.atom_name);
      break;
   case ic_pid:
#ifndef __OTP_PRE_23__
      retVal = t2->type == ic_pid &&
	 !ei_cmp_pids(t1->value.pid, t2->value.pid);
#else
      retVal = t2->type == ic_pid &&
	 t1->value.pid->num == t2->value.pid->num &&
	 t1->value.pid->serial == t2->value.pid->serial &&
	 t1->value.pid->creation == t2->value.pid->creation &&
	 !strcmp(t1->value.pid->node,
		 t2->value.pid->node);
#endif
      break;
   case ic_port:
#ifndef __OTP_PRE_23__
      retVal = t2->type == ic_port &&
	 !ei_cmp_ports(t1->value.port, t2->value.port);
#else
      retVal = t2->type == ic_port &&
	 t1->value.port->id == t2->value.port->id &&
	 t1->value.port->creation == t2->value.port->creation &&
	 !strcmp(t1->value.port->node,
		 t2->value.port->node);
#endif
      break;
   case ic_ref:
#ifndef __OTP_PRE_23__
      retVal = t2->type == ic_ref &&
	 !ei_cmp_refs(t1->value.ref, t2->value.ref);
#else
      retVal = t2->type == ic_ref &&
	 t1->value.ref->len == t2->value.ref->len &&
	 t1->value.pid->creation == t2->value.pid->creation &&
	 !strcmp(t1->value.pid->node,
		 t2->value.pid->node);
      if(retVal) {
	 for (i = 0; i < t1->value.ref->len; i++)
	    if (t1->value.ref->n[i] != t2->value.ref->n[i])
	       return 0;
      }
#endif
      break;
   case ic_tuple:
      if(t2->type == ic_tuple &&
	 t1->value.tuple->arity == t2->value.tuple->arity)
      {
	 for(i = 0; i < t1->value.tuple->arity; i++) {
	    retVal = ic_erlang_term_is_equal(t1->value.tuple->elements[i],
					     t2->value.tuple->elements[i]);
	    if(retVal == 0)
	       break;
	 }
      } else
	 retVal = 0;
      break;
   case ic_list:
      if(t2->type == ic_list &&
	 t1->value.list->arity == t2->value.list->arity) {
	 if(t1->value.list->arity)
	    retVal = ic_compare_list(t1->value.list, t2->value.list);
      } else
	 retVal = 0;
      break;
   case ic_binary:
      if(t2->type == ic_binary &&
	 t1->value.bin->size == t2->value.bin->size &&
	 !memcmp(t1->value.bin->bytes,
		 t2->value.bin->bytes,
		 t1->value.bin->size))
	 retVal = 1;
      else
	 retVal = 0;
      break;
   default:
      break;
   }

   return retVal;
}

int ic_compare_list(ic_erlang_list *list1, ic_erlang_list *list2)
{
   int retVal = 1;
   ic_erlang_list_elem *p1 = NULL;
   ic_erlang_list_elem *p2 = NULL;

   /* Already checked that the arity is the same */
   for(p1 = list1->head, p2 = list2->head;
       p1;
       p1 = p1->next, p2 = p2->next) {
      retVal = ic_erlang_term_is_equal(p1->element, p2->element);
      if(retVal == 0)
	 break;
   }
   return retVal;
}


/* Create functions */
ic_erlang_term* ic_mk_int_term(long l)
{
   ic_erlang_term *term = NULL;

   term = (ic_erlang_term*) malloc(sizeof(ic_erlang_term));
   if(term) {
      term->type = ic_integer;
      term->value.i_val = l;
      term->_one_block_alloc = 0;
   }
   
   return term;
}

ic_erlang_term* ic_mk_float_term(double d)
{
   ic_erlang_term *term = NULL;

   term = (ic_erlang_term*) malloc(sizeof(ic_erlang_term));
   if(term) {
      term->type = ic_float;
      term->value.d_val = d;
      term->_one_block_alloc = 0;
   }
   return term;
}

ic_erlang_term* ic_mk_atom_term(char *atom_name)
{
   ic_erlang_term *term = NULL;
   int len;
   char *str;

   if(atom_name) {
      len = strlen(atom_name) + 1;
      if(len <= MAXATOMLEN_UTF8) {
	 term = (ic_erlang_term*) malloc(sizeof(ic_erlang_term));
	 if(term) {
	    term->type = ic_atom;
	    term->_one_block_alloc = 0;
	    str = (char*) malloc(len);
	    if(str) {
	       strcpy(str, atom_name);
	       term->value.atom_name = str;
	    } else {
	       CORBA_free(term);
	       term = NULL;
	    }
	 }
      }
   }
   return term;
}

ic_erlang_term* ic_mk_pid_term(erlang_pid* pid)
{
   ic_erlang_term *term = NULL;
   erlang_pid *pid_data = NULL;

   if(pid) {
      pid_data = (erlang_pid *) malloc(sizeof(erlang_pid));
      if(pid_data) {
	 *pid_data = *pid;
	 term = (ic_erlang_term*) malloc(sizeof(ic_erlang_term));
	 if(term) {
	    term->type = ic_pid;
	    term->value.pid = pid_data;
	    term->_one_block_alloc = 0;
	 } else
	    free(pid_data);
      }
   }
   return term;
}

ic_erlang_term* ic_mk_port_term(erlang_port* port)
{
   ic_erlang_term *term = NULL;
   erlang_port *port_data = NULL;

   if(port) {
      port_data = (erlang_port *) malloc(sizeof(erlang_port));
      if(port_data) {
	 *port_data = *port;
	 term = (ic_erlang_term *) malloc(sizeof(ic_erlang_term));
	 if(term) {
	    term->type = ic_port;
	    term->value.port = port_data;
	    term->_one_block_alloc = 0;
	 } else
	    free(port_data);
      }
   }
   return term;
}

ic_erlang_term* ic_mk_ref_term(erlang_ref* ref)
{
   ic_erlang_term *term = NULL;
   erlang_ref *ref_data = NULL;

   if(ref) {
      ref_data = (erlang_ref *) malloc(sizeof(erlang_ref));
      if(ref_data) {
	 *ref_data = *ref;
	 term = (ic_erlang_term *) malloc(sizeof(ic_erlang_term));
	 if(term) {
	    term->type = ic_ref;
	    term->value.ref = ref_data;
	    term->_one_block_alloc = 0;
	 } else
	    free(ref_data);
      }
   }
   return term;
}

ic_erlang_term* ic_mk_tuple_term(int arity)
{
   ic_erlang_term *term = NULL;
   ic_erlang_tuple *tuple;

   term = (ic_erlang_term *) malloc(sizeof(ic_erlang_term));
   if(term) {
      term->type = ic_tuple;
      term->_one_block_alloc = 0;
      tuple = (ic_erlang_tuple*) malloc(sizeof(ic_erlang_tuple));
      if(tuple) {
	 term->value.tuple = tuple;
	 tuple->arity = arity;
	 if(arity) {
	    tuple->elements = (ic_erlang_term**) malloc(sizeof(ic_erlang_term*) * arity);
	    if(!tuple->elements) {
	       CORBA_free(term);
	       term = NULL;
	    }
	 } else
	    tuple->elements = NULL;
      } else {
	 CORBA_free(term);
	 term = NULL;
      }
   }
   return term;
}

int ic_tuple_add_elem(ic_erlang_term *tuple, ic_erlang_term *elem, int pos)
{
   int retVal = 0;

   if(tuple && elem)
      tuple->value.tuple->elements[pos] = elem;
   else
      retVal = -1;

   return retVal;
}

ic_erlang_term* ic_mk_list_term(void)
{
   ic_erlang_term *term = NULL;
   ic_erlang_list *list;

   term = (ic_erlang_term *) malloc(sizeof(ic_erlang_term));
   if(term) {
      term->type = ic_list;
      term->_one_block_alloc = 0;
      list = (ic_erlang_list*) malloc(sizeof(ic_erlang_list));
      if(list) {
	 term->value.list = list;
	 list->arity = 0;
	 list->head = NULL;
	 list->tail = NULL;
      } else {
	 CORBA_free(term);
	 term = NULL;
      }
   }
   return term;
}

int ic_list_add_elem(ic_erlang_term *list, ic_erlang_term *term)
{
   int retVal = 0;
   ic_erlang_list_elem *elem = NULL;

   if(list && term) {
      elem = (ic_erlang_list_elem *) malloc(sizeof(ic_erlang_list_elem));
      if(elem) {
	 elem->element = term;
	 elem->next = NULL;

	 list->value.list->arity += 1;
	 /* fprintf(stderr, "New length: %ld\n", list->value.list.arity); */

	 if(list->value.list->head) {
	    /* fprintf(stderr, "Add elem\n"); */
	    list->value.list->tail->next = elem;
	    list->value.list->tail = elem;
	 } else {
	    /* fprintf(stderr, "First elem\n"); */
	    list->value.list->head = elem;
	    list->value.list->tail = elem;
	 }
      } else
	 retVal = -1;
   } else
      retVal = -1;

   return retVal;
}

ic_erlang_term* ic_mk_list_term_from_string(char *str)
{
   ic_erlang_term *list = NULL;
   ic_erlang_term *tmp_term;
   int length, i;

   if(str) {
      length = strlen(str);
      list = ic_mk_list_term();
      for(i = 0; i < length; i++) {
	 tmp_term = ic_mk_int_term((long) str[i]);
	 if(ic_list_add_elem(list, tmp_term) == -1) {
	    ic_free_erlang_term(list);
	    list = NULL;
	    goto error;
	 }
      }
   }

 error:
   return list;
}

ic_erlang_term* ic_mk_binary_term(int size, char *b)
{
   ic_erlang_term *term = NULL;
   ic_erlang_binary *bin;

   if(b) {
      term = (ic_erlang_term *) malloc(sizeof(ic_erlang_term));
      if(term) {
	 term->type = ic_binary;
	 term->_one_block_alloc = 0;
	 bin = (ic_erlang_binary*) malloc(sizeof(ic_erlang_binary));
	 if(bin) {
	    term->value.bin = bin;
	    bin->size = size;
	    bin->bytes = (char*) malloc(size);
	    if(bin->bytes)
	       memcpy(bin->bytes, b, size);
	    else {
	       ic_free_erlang_term(term);
	       term = NULL;
	    }
	 } else {
	    ic_free_erlang_term(term);
	    term = NULL;
	 }
      }
   }
   return term;
}

/* Free functions */
void ic_free_erlang_term(ic_erlang_term* term)
{
   if(term) {
      if(!(term->_one_block_alloc))
	 switch(term->type) {
	 case ic_integer:
	 case ic_float:
	    break;
	 case ic_atom:
	    CORBA_free(term->value.atom_name);
	    break;
	 case ic_pid:
	    CORBA_free(term->value.pid);
	    break;
	 case ic_port:
	    CORBA_free(term->value.port);
	    break;
	 case ic_ref:
	    CORBA_free(term->value.ref);
	    break;
	 case ic_tuple:
	    ic_free_tuple(term->value.tuple);
	    break;
	 case ic_list:
	    ic_free_list(term->value.list);
	    break;
	 case ic_binary:
	    CORBA_free(term->value.bin->bytes);
	    CORBA_free(term->value.bin);
	    break;
	 default:
	    break;
	 }

      CORBA_free(term);
   }
   return;
}

void ic_free_tuple(ic_erlang_tuple *tuple)
{
   int i;

   if(tuple) {
      for(i = 0; i < tuple->arity; i++)
	 ic_free_erlang_term(tuple->elements[i]);

      CORBA_free(tuple->elements);
      CORBA_free(tuple);
   }
   return;
}

void ic_free_list(ic_erlang_list *list)
{
   ic_erlang_list_elem *p = NULL,
      *q = NULL;

   if(list) {
      for(p = list->head; p; p = q) {
	 q = p->next;
	 ic_free_erlang_term(p->element);
	 CORBA_free(p);
      }
      CORBA_free(list);
   }

   return;
}

/* Print functions */
void ic_print_erlang_term(ic_erlang_term *term)
{

   if(!term) {
      fprintf(stdout, "Null pointer\n");
      return;
   }

   switch(term->type) {

   case ic_integer:
      fprintf(stdout, "Type: ic_integer, Value: %ld\n", term->value.i_val);
      break;
   case ic_float:
      fprintf(stdout, "Type: ic_float, Value: %f\n", term->value.d_val);
      break;
   case ic_atom:
      fprintf(stdout, "Type: ic_atom, Value: %s\n", term->value.atom_name);
      break;
   case ic_pid:
      fprintf(stdout, "Type: ic_pid, Value: %s %d %d %d\n",
	      term->value.pid->node,
	      term->value.pid->num,
	      term->value.pid->serial,
	      term->value.pid->creation);
      break;
   case ic_port:
      fprintf(stdout, "Type: ic_port, Value: %s %lld %d\n",
	      term->value.port->node,
	      (long long) term->value.port->id,
	      term->value.port->creation);
      break;
   case ic_ref:
      fprintf(stdout, "Type: ic_ref, Value: %s %d [%d,%d,%d] %d\n",
	      term->value.ref->node,
	      term->value.ref->len,
	      term->value.ref->n[0],
	      term->value.ref->n[1],
	      term->value.ref->n[2],
	      term->value.ref->creation);
      break;
   case ic_tuple:
      ic_print_tuple(term->value.tuple);
      break;
   case ic_list:
      ic_print_list(term->value.list);
      break;
   case ic_binary:
       fprintf(stdout, "Type: ic_binary, Size: %ld\n",
	       term->value.bin->size);
      break;
   default:

      break;
   }

   return;
}

void ic_print_tuple(ic_erlang_tuple *tuple)
{
   int i;

   fprintf(stdout, "Type: ic_tuple, Arity: %ld\n", tuple->arity);
   for(i = 0; i < tuple->arity; i++)
      ic_print_erlang_term(tuple->elements[i]);

   return;
}

void ic_print_list(ic_erlang_list *list)
{
   ic_erlang_list_elem *p = NULL;

   fprintf(stdout, "Type: ic_list, Arity: %ld\n", list->arity);

   for(p = list->head; p; p = p->next)
      ic_print_erlang_term(p->element);

   return;
}
