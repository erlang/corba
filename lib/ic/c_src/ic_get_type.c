/*
 * %CopyrightBegin%
 *
 * Copyright Ericsson AB 2020-2020. All Rights Reserved.
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
 *
 */
#include <ic.h>

int ic_get_type(const char *buf, const int *index, ic_erlang_type *ictype, int *len)
{
   int retVal;
   int type;

   retVal = ei_get_type(buf, index, &type, len);
   if(retVal)
      return retVal;

   switch(type) {
      
   case ERL_SMALL_INTEGER_EXT:
   case ERL_INTEGER_EXT:
   case ERL_SMALL_BIG_EXT:
   case ERL_LARGE_BIG_EXT:
      *ictype = ic_integer;
      break;

   case ERL_FLOAT_EXT:
      *ictype = ic_float;
      break;
      
   case ERL_ATOM_EXT:
      *ictype = ic_atom;
      break;

   case ERL_STRING_EXT:
      *ictype = ic_string;
      break;

   case ERL_PID_EXT:
      *ictype = ic_pid;
      break;
      
   case ERL_PORT_EXT:
      *ictype = ic_port;
      break;
      
   case ERL_NEW_REFERENCE_EXT:
      *ictype = ic_ref;
      break;
      
   case ERL_SMALL_TUPLE_EXT:
   case ERL_LARGE_TUPLE_EXT:
      *ictype = ic_tuple;
      break;
      
   case ERL_NIL_EXT: /* List of length 0 */
   case ERL_LIST_EXT:
      *ictype = ic_list;
      break;
      
  case ERL_BINARY_EXT:
      *ictype = ic_binary;
      break;

   default: /* ic term doesn't handle maps, bit binaries, funs ... */
      retVal = -1;
      break;
   }

   return retVal;
}
