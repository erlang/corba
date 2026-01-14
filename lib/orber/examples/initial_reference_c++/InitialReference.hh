/*
 * %CopyrightBegin%
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Copyright Ericsson AB 1997-2026. All Rights Reserved.
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
/*
 * InitialReference is a class for creating an external IOR for the object
 * reference INIT.
 */
#ifndef _INITIALREFERENCE_HH
#define _INITIALREFERENCE_HH
#include <stdio.h>
#include <string.h>

#if HAVE_SSTREAM
#include <sstream>
typedef std::stringstream STRINGSTREAM;
typedef std::stringbuf STRINGBUF;
#else
#include <strstream.h>
typedef strstream STRINGSTREAM;
typedef strstreambuf STRINGBUF;
#endif

class InitialReference {
private:
  char* iorString;
  char* host;
  int port;

  void enc_ushort(int s, char *byteArray);
  void enc_ulong(long l, char *byteArray);
  void createIOR(STRINGSTREAM& byte, long length);
  long align(STRINGBUF* sbuf, long currentLength, int alignment);

public:
  InitialReference();
  ~InitialReference();

  char* stringified_ior(char* host, int port);
  
};

#endif
