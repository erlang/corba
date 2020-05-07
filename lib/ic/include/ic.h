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
 *
 */

#ifndef _IC_H
#define _IC_H


#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ei.h>

#ifdef __WIN32__
/* Windows.h #defines interface to struct, get rid of it! */
#ifdef interface
#undef interface
#endif
#endif

#ifndef __IC_H__
#define __IC_H__

#ifdef __cplusplus
extern "C" {
#endif

/* This declaration is just used in pre OTP 23 code */
#ifdef __OTP_PRE_23__
short ei_thiscreation(const ei_cnode* ec);
#endif

/* -------------------------------------------------------------------- */
/* Type definitions of Erlang terms in C                                */
/* -------------------------------------------------------------------- */

/* ic_string is just used internally during decoding when checking      */
/*    next type. It's not used in ic_erlang_term.                       */
typedef enum {
   ic_integer,
   ic_float,
   ic_atom,
   ic_pid,
   ic_port,
   ic_ref,
   ic_tuple,
   ic_string,
   ic_list,
   ic_binary
} ic_erlang_type;

struct _ic_erlang_term; /* forward */

typedef struct _ic_erlang_tuple {
   long arity;
   struct _ic_erlang_term **elements;
} ic_erlang_tuple;

typedef struct _ic_erlang_list_elem {
   struct _ic_erlang_term *element;
   struct _ic_erlang_list_elem *next;
} ic_erlang_list_elem;

typedef struct _ic_erlang_list {
   long arity;
   struct _ic_erlang_list_elem *head;
   struct _ic_erlang_list_elem *tail;
} ic_erlang_list;

typedef struct _ic_erlang_binary {
   long size;
   char* bytes;
} ic_erlang_binary;

typedef struct _ic_erlang_term {
   ic_erlang_type type;
   union {
      long              i_val;
      double            d_val;
      char*             atom_name;
      erlang_pid*       pid;
      erlang_port*      port;
      erlang_ref*       ref;
      ic_erlang_tuple*  tuple;
      ic_erlang_list*   list;
      ic_erlang_binary* bin;
   } value;
} ic_erlang_term;

/* Decode/Encode of erlang terms */
int ic_encode_term(char *buf, int *index, const ic_erlang_term *term);
int ic_decode_term(const char *buf, int *index, ic_erlang_term **term);
int ic_size_of_encoded_term(int *index, const ic_erlang_term *term);
int ic_get_type(const char *buf, const int *index, ic_erlang_type *type, int *len);

/* Help functions for erlang terms */
int ic_erlang_term_is_equal(ic_erlang_term *t1, ic_erlang_term *t2);

ic_erlang_term* ic_mk_int_term(long l);
ic_erlang_term* ic_mk_float_term(double d);
ic_erlang_term* ic_mk_atom_term(char *atom_name);
ic_erlang_term* ic_mk_pid_term(erlang_pid* pid);
ic_erlang_term* ic_mk_port_term(erlang_port* port);
ic_erlang_term* ic_mk_ref_term(erlang_ref* ref);
ic_erlang_term* ic_mk_tuple_term(int arity);
int ic_tuple_add_elem(ic_erlang_term *tuple, ic_erlang_term *elem, int pos);
ic_erlang_term* ic_mk_list_term(void);
int ic_list_add_elem(ic_erlang_term *list, ic_erlang_term *term);
ic_erlang_term* ic_mk_list_term_from_string(char *str);
ic_erlang_term* ic_mk_binary_term(int size, char *b);

void ic_free_erlang_term(ic_erlang_term *term);
void ic_print_erlang_term(ic_erlang_term *term);

/* Standard type mapping */

#ifndef __CORBA_SHORT__
#define __CORBA_SHORT__
    typedef short           CORBA_short;
#endif

#ifndef __CORBA_LONG__
#define __CORBA_LONG__
    typedef long            CORBA_long;
#endif

/* CORBA_long_long = long because of erl_interface limitation */
#ifndef __CORBA_LONG_LONG__
#define __CORBA_LONG_LONG__
    typedef long            CORBA_long_long;  /* LONG LONG */
#endif

#ifndef __CORBA_UNSIGNED_SHORT__
#define __CORBA_UNSIGNED_SHORT__
    typedef unsigned short  CORBA_unsigned_short;
#endif

#ifndef __CORBA_UNSIGNED_LONG__
#define __CORBA_UNSIGNED_LONG__
    typedef unsigned long CORBA_unsigned_long;
#endif

/* CORBA_unsigned long_long = unsigned long because of erl_interface
   limitation */

#ifndef __CORBA_UNSIGNED_LONG_LONG__
#define __CORBA_UNSIGNED_LONG_LONG__
    typedef unsigned long CORBA_unsigned_long_long;
#endif

#ifndef __CORBA_FLOAT__
#define __CORBA_FLOAT__
    typedef float           CORBA_float;
#endif

#ifndef __CORBA_DOUBLE__
#define __CORBA_DOUBLE__
    typedef double          CORBA_double;
#endif


#ifndef __CORBA_LONG_DOUBLE__
#define __CORBA_LONG_DOUBLE__
    typedef double          CORBA_long_double;
#endif

#ifndef __CORBA_CHAR__
#define __CORBA_CHAR__
    typedef char            CORBA_char;
#endif

#ifndef __CORBA_WCHAR__
#define __CORBA_WCHAR__
    typedef unsigned long   CORBA_wchar;
#endif

#ifndef __CORBA_BOOLEAN__
#define __CORBA_BOOLEAN__
    typedef unsigned char   CORBA_boolean;
#endif

#ifndef __CORBA_OCTET__
#define __CORBA_OCTET__
    typedef char            CORBA_octet;
#endif

#ifndef CORBA_enum
#define CORBA_enum      enum
#endif

#ifndef __ERLANG_BINARY__
#define __ERLANG_BINARY__
    typedef struct {
	CORBA_unsigned_long _maximum;
	CORBA_unsigned_long _length;
	CORBA_octet* _buffer;
    } erlang_binary;
#endif


/* Object  definition */
    typedef void* CORBA_Object;


/* Exception discriminators */
#ifndef CORBA_NO_EXCEPTION
#define CORBA_NO_EXCEPTION      0
#endif

#ifndef CORBA_SYSTEM_EXCEPTION
#define CORBA_SYSTEM_EXCEPTION -1
#endif

#ifndef CORBA_USER_EXCEPTION
#define CORBA_USER_EXCEPTION   -2
#endif

/* System exceptions */

#define UNKNOWN          "UNKNOWN"
#define BAD_PARAM        "BAD_PARAM"
#define NO_MEMORY        "NO_MEMORY"
#define IMPL_LIMIT       "IMP_LIMIT"
#define COMM_FAILURE     "COMM_FAILURE"
#define INV_OBJREF       "INV_OBJREF"
#define NO_PERMISSION    "NO_PERMISSION"
#define INTERNAL         "INTERNAL"
#define MARSHAL          "MARSHAL"
#define INITIALIZE       "INITIALIZE"
#define NO_IMPLEMENT     "NO_IMPLEMENT"
#define BAD_TYPECODE     "BAD_TYPECODE"
#define BAD_OPERATION    "BAD_OPERATION"
#define NO_RESOURCES     "NO_RESOURCES"
#define NO_RESPONSE      "NO_RESPONSE"
#define PERSIST_STORE    "PERSIST_STORE"
#define BAD_INV_ORDER    "BAD_INV_ORDER"
#define TRANSIENT        "TRANSIENT"
#define FREE_MEM         "FREE_MEM"
#define INV_IDENT        "INV_IDENT"
#define INV_FLAG         "INV_FLAG"
#define INTF_REPOS       "INTF_REPOS"
#define BAD_CONTEXT      "BAD_CONTEXT"
#define OBJ_ADAPTER      "OBJ_ADAPTER"
#define DATA_CONVERSION  "DATA_CONVERSION"
#define OBJ_NOT_EXIST    "OBJECT_NOT_EXIST"



/* Exception type */
    typedef int CORBA_exception_type;


#ifndef __CORBA_ENVIRONMENT__
#define __CORBA_ENVIRONMENT__

/* Environment definition */
    typedef struct {

	/*----- CORBA compatibility part ------------------------------------*/
	CORBA_exception_type   _major;          /* Exception tag, initially set
						   to CORBA_NO_EXCEPTION   */

	/*----- External Implementation part - initiated by the user --------*/
	int                    _fd;             /* File descriptor           */
	int                    _inbufsz;        /* Size of input buffer      */
	char                  *_inbuf;          /* Pointer to always
						   dynamically allocated
						   buffer for input   */
	int                    _outbufsz;       /* Size of output buffer     */
	char                  *_outbuf;         /* Pointer to always
						   dynamically
						   allocated buffer
						   for output */
	int                    _memchunk;       /* Size of memory
						   chunks in bytes,
						   used for increasing
						   the output buffer,
						   set to >= 32,
						   should be around >=
						   1024 for
						   performance reasons */
	char                   _regname[256];   /* Pointer for
                                                   registered name */
	erlang_pid            *_to_pid;         /* Process identity
                                                   for caller */
	erlang_pid            *_from_pid;       /* Process identity
                                                   for callee */
	/*----- Internal Implementation part - used by the server/client ----*/
	int                    _iin;            /* Index for input buffer */
	int                    _iout;           /* Index for output buffer */
	char                   _operation[256]; /* Pointer for operation name*/
	int                    _received;       /* Used to count parameters */
	erlang_pid             _caller;         /* Used to identify
                                                   the caller*/
	erlang_ref             _unique;         /* Used to identify the call */
	CORBA_char            *_exc_id;         /* Exception id field        */
	void                  *_exc_value;      /* Exception value field     */

	unsigned int          _ref_counter_1;   /* Counter for reference     */
	unsigned int          _ref_counter_2;   /* Counter for reference     */
	unsigned int          _ref_counter_3;   /* Counter for reference     */
        ei_cnode              *_ec;              /* Pointer to the cnode */
    } CORBA_Environment;

#endif

/* Corba standard functions */

    void CORBA_free(void *);
    CORBA_char *CORBA_string_alloc(CORBA_unsigned_long);
    CORBA_wchar *CORBA_wstring_alloc(CORBA_unsigned_long);
    CORBA_char *CORBA_exception_id(CORBA_Environment *env);
    void *CORBA_exception_value(CORBA_Environment *env);
    void CORBA_exception_free(CORBA_Environment *env);
    void CORBA_exc_set(CORBA_Environment *env,
		       CORBA_exception_type Major,
		       CORBA_char *Id,
		       CORBA_char *Value);
    CORBA_Environment *CORBA_Environment_alloc(int inbufsz, int outbufsz);
    void ic_init_ref(CORBA_Environment *env, erlang_ref *ref);
    int ic_compare_refs(erlang_ref *ref1, erlang_ref *ref2);

/* Used internally */

#define __OE_MEMCHUNK__   1024
#define __OE_VSNSZ__         1
#define __OE_LONGSZ__        (3+sizeof(long))
#define __OE_LONGLONGSZ__    (3+sizeof(long))
#define __OE_ULONGSZ__       (3+sizeof(long))
#define __OE_ULONGLONGSZ__   (3+sizeof(long))
#define __OE_DOUBLESZ__      9
#define __OE_CHARSZ__        2
#define __OE_WCHARSZ__       (3+sizeof(CORBA_wchar))
#define __OE_TUPLEHDRSZ__    5
#define __OE_LISTHDRSZ__     5

/* The actual size of a wide char (used to be #define __OE_WCHAR_SIZE_OF__ 4) */
#define __OE_WCHAR_SIZE_OF__ sizeof(CORBA_wchar)

/* Size check macro */
#define OE_MALLOC_SIZE_CHECK(env,x) { \
    assert((x) > 0); \
    if (!((x) > 0)) { \
        CORBA_exc_set((env), CORBA_SYSTEM_EXCEPTION, INTERNAL, \
            "Bad malloc size calculation"); \
        return -1; \
    } \
}

/* Exec function -- probably not needed */
    typedef int oe_exec_function_t(CORBA_Object, CORBA_Environment*);
/* These are for backward compatibility */
    typedef oe_exec_function_t ___exec_function___;
    typedef oe_exec_function_t ___generic___;

/* Operation declaration */
    typedef struct {
	char *interface;
	char *name;
	oe_exec_function_t  *function;
    } oe_operation_t;

/* For backward compatibility */
    typedef oe_operation_t ___operation___;

/* Map declaration */
    typedef struct {
	int length;
	oe_operation_t *operations;
    } oe_map_t;
/* For backward compatibility */
    typedef oe_map_t ___map___;

/* Align macro */
#define OE_ALIGN(x) (((x) + sizeof(double) - 1) & ~(sizeof(double) - 1))

/* Encoders */
    int oe_ei_encode_version(CORBA_Environment *env);
    int oe_ei_encode_long(CORBA_Environment *env, long p);
    int oe_ei_encode_longlong(CORBA_Environment *env, CORBA_long_long p);
    int oe_ei_encode_ulong(CORBA_Environment *env, unsigned long p);
    int oe_ei_encode_ulonglong(CORBA_Environment *env,
			       CORBA_unsigned_long_long p);
    int oe_ei_encode_double(CORBA_Environment *env, double p);
    int oe_ei_encode_char(CORBA_Environment *env, char p);
    int oe_ei_encode_wchar(CORBA_Environment *env, CORBA_wchar p);
    int oe_ei_encode_string(CORBA_Environment *env, const char *p);
    int oe_ei_encode_wstring(CORBA_Environment *env, CORBA_wchar *p);
    int oe_ei_encode_atom(CORBA_Environment *env, const char *p);
    int oe_ei_encode_pid(CORBA_Environment *env, const erlang_pid *p);
    int oe_ei_encode_port(CORBA_Environment *env, const erlang_port *p);
    int oe_ei_encode_ref(CORBA_Environment *env, const erlang_ref *p);
    int oe_ic_encode_term(CORBA_Environment *env, ic_erlang_term *t);
    int oe_ei_encode_tuple_header(CORBA_Environment *env, int arity);
    int oe_ei_encode_list_header(CORBA_Environment *env, int arity);
    int oe_encode_erlang_binary(CORBA_Environment *env, erlang_binary *binary);

#define oe_ei_encode_empty_list(ev) oe_ei_encode_list_header(ev,0)

/* Decoders */
    int oe_ei_decode_wchar(const char *buf, int *index, CORBA_wchar *p);
    int oe_ei_decode_wstring(const char *buf, int *index, CORBA_wchar *p);
    int oe_ei_decode_longlong(const char *buf, int *index, CORBA_long_long *p);
    int oe_ei_decode_ulonglong(const char *buf, int *index,
			       CORBA_unsigned_long_long *p);
    int oe_decode_erlang_binary(CORBA_Environment *env, char *buf, int *index,
				erlang_binary *binary);

/* Generic client encoders (gen_server protocol) */
    int oe_prepare_notification_encoding(CORBA_Environment *env);
    int oe_prepare_request_encoding(CORBA_Environment *env);

/* Generic client decoders (gen_server protocol) */
    int oe_prepare_reply_decoding(CORBA_Environment *env);

/* Generic client send and receive functions (Erlang distribution protocol) */
    int oe_send_notification(CORBA_Environment *env);
    int oe_send_notification_tmo(CORBA_Environment *env, unsigned int send_ms);
    int oe_send_request_and_receive_reply(CORBA_Environment *env);
    int oe_send_request_and_receive_reply_tmo(CORBA_Environment *env,
					      unsigned int send_ms,
					      unsigned int recv_ms);

/* Generic server decoder */
    int oe_prepare_request_decoding(CORBA_Environment *env);

/* Generic server encoder */
    int oe_prepare_reply_encoding(CORBA_Environment *env);

/* -------- */

/* Generic server receive (possibly send reply) */
    int oe_server_receive(CORBA_Environment *env, oe_map_t *map);
    int oe_server_receive_tmo(CORBA_Environment *env, oe_map_t *map,
			      unsigned int send_ms,
			      unsigned int recv_ms);

/* -------- */

/* Size calculators */
    int oe_sizecalc_erlang_binary(CORBA_Environment *env, int *index,
				  int *size);
/* Print functions */
    int print_erlang_binary(erlang_binary*);

/* Length counter for wide strings */
    int ic_wstrlen(CORBA_wchar * p);

/* Wide string comparison */
    int ic_wstrcmp(CORBA_wchar * ws1, CORBA_wchar * ws2);

/* Put for 64-bits integer type */
#define put64le(s,n) do { \
  (s)[0] = (n) & 0xff;  \
  (s)[1] = ((n) >>  8) & 0xff; \
  (s)[2] = ((n) >>  16) & 0xff; \
  (s)[3] = ((n) >>  24) & 0xff; \
  (s)[4] = ((n) >>  32) & 0xff; \
  (s)[5] = ((n) >>  40) & 0xff; \
  (s)[6] = ((n) >>  48) & 0xff; \
  (s)[7] = ((n) >>  56) & 0xff; \
  (s)[8] = ((n) >>  64) & 0xff; \
  (s) += 8; \
} while (0)

/* Get for 64-bits integer type */
#define get64le(s) \
  ((s) += 8, \
   ((((unsigned char *)(s))[-1] << 56) | \
    (((unsigned char *)(s))[-2] << 48) | \
    (((unsigned char *)(s))[-3] << 40) | \
    (((unsigned char *)(s))[-4] << 32) | \
    (((unsigned char *)(s))[-5] << 24) | \
    (((unsigned char *)(s))[-6] << 16) | \
    (((unsigned char *)(s))[-7] << 8) | \
    ((unsigned char *)(s))[-8]))



/* Exec function switch */
    int oe_exec_switch(CORBA_Object, CORBA_Environment*, oe_map_t*);
/* For backward compatibility */
    int ___switch___(CORBA_Object, CORBA_Environment*, oe_map_t*);

/* For backward compatibility -- replaced by oe_prepare_request_decoding() */
    int ___call_info___(CORBA_Object, CORBA_Environment*);

/* Map merging */
    oe_map_t* oe_merge_maps(oe_map_t*, int);
/* For backward compatibility */
    oe_map_t* ___merge___(oe_map_t*, int);

/* Macro for error reporting */

#ifdef OE_C_REPORT
#define OE_RPT_ERR(x)  fprintf(stderr, (x))
#else
#define OE_RPT_ERR(x)
#endif

#ifdef __cplusplus
}
#endif

#endif

#endif /* IC_H */
