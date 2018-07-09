#
# %CopyrightBegin%
# 
# Copyright Ericsson AB 1998-2016. All Rights Reserved.
# 
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
# 
# %CopyrightEnd%
#
#
include $(ERL_TOP)/make/target.mk
include $(ERL_TOP)/make/$(TARGET)/otp.mk

include $(ERL_TOP)/make/$(TARGET)/otp_ded.mk

CORBA_TOP=../../..

FRD = 'io:fwrite("~n=:~s~n",[code:lib_dir(erl_interface)]),init:stop().'
EI_DIR := $(shell echo $(FRD) | erl | sed -n '/^=:/s/^=://p')

CC = $(DED_CC)

PRIVDIR = ../priv
OBJDIR = $(PRIVDIR)/obj/$(TARGET)
LIBDIR = $(PRIVDIR)/lib/$(TARGET)

INCDIR = ../include

ERL_INTERFACE_FLAGS = \
	-I$(EI_DIR)/include

# ----------------------------------------------------
# Application version
# ----------------------------------------------------
include $(CORBA_TOP)/vsn.mk
VSN=$(CORBA_VSN)

# ----------------------------------------------------
# Release directory specification
# ----------------------------------------------------
RELSYSDIR = $(RELEASE_PATH)/lib/ic-$(VSN)

# ----------------------------------------------------
# File Specs
# ----------------------------------------------------

IDL_FILES = \
        $(INCDIR)/erlang.idl

ifeq ($(findstring win32,$(TARGET)),win32)
ifeq ($(MIXED_CYGWIN_MINGW),yes)
AR_OUT = rcv
CC_FLAGS =
LIBRARY = $(LIBDIR)/libic.a
SKIP_BUILDING_BINARIES := false
else
LIBRARY = $(LIBDIR)/ic.lib
AR_OUT = -out:
CC_FLAGS = -MT
endif

else

ifeq ($(HOST_OS),)
HOST_OS := $(shell $(ERL_TOP)/erts/autoconf/config.guess)
endif
ifeq ($(findstring solaris,$(HOST_OS)),solaris)
SKIP_BUILDING_BINARIES := true
endif
ifeq ($(V),0)
AR_OUT = rc
else
AR_OUT = rcv
endif
CC_FLAGS = $(DED_CFLAGS)
LIBRARY = $(LIBDIR)/libic.a
SKIP_BUILDING_BINARIES := false

endif

C_FILES = \
	ic.c \
	ic_tmo.c \
	oe_ei_encode_version.c      \
	oe_ei_encode_long.c         \
	oe_ei_encode_ulong.c        \
	oe_ei_encode_double.c       \
	oe_ei_encode_char.c         \
	oe_ei_encode_string.c       \
	oe_ei_encode_atom.c         \
	oe_ei_encode_pid.c          \
	oe_ei_encode_port.c         \
	oe_ei_encode_ref.c          \
	oe_ei_encode_term.c         \
	oe_ei_encode_tuple_header.c \
	oe_ei_encode_list_header.c  \
	oe_ei_encode_longlong.c     \
	oe_ei_encode_ulonglong.c    \
	oe_ei_encode_wchar.c        \
	oe_ei_encode_wstring.c      \
	oe_ei_decode_longlong.c     \
	oe_ei_decode_ulonglong.c    \
	oe_ei_decode_wchar.c        \
	oe_ei_decode_wstring.c      \
	oe_ei_code_erlang_binary.c  

H_FILES = $(INCDIR)/ic.h

OBJ_FILES= $(C_FILES:%.c=$(OBJDIR)/%.o) 

ALL_CFLAGS = $(BASIC_CFLAGS) -I$(INCDIR) $(ERL_INTERFACE_FLAGS)

# ----------------------------------------------------
# Targets
# ----------------------------------------------------

ifeq ($(SKIP_BUILDING_BINARIES), true)
debug opt:
else
debug opt: $(LIBRARY)
endif

clean:
	rm -f $(LIBRARY) $(OBJ_FILES) 
	rm -f core *~

docs:

# ----------------------------------------------------
# Special Build Targets
# ----------------------------------------------------

_create_dirs := $(shell mkdir -p $(OBJDIR) $(LIBDIR))

$(LIBRARY): $(OBJ_FILES) 
	-$(V_AR) $(AR_OUT) $@ $(OBJ_FILES)
	-$(V_RANLIB) $@

$(OBJDIR)/%.o: %.c
	$(V_CC) $(CC_FLAGS) -c -o $@ $(ALL_CFLAGS) $<

# ----------------------------------------------------
# Release Target
# ---------------------------------------------------- 
include $(ERL_TOP)/make/otp_release_targets.mk

release_spec: opt
	$(INSTALL_DIR) "$(RELSYSDIR)/c_src"
	$(INSTALL_DIR) "$(RELSYSDIR)/include"
	$(INSTALL_DIR) "$(RELSYSDIR)/priv/lib"
	$(INSTALL_DATA) ic.c ic_tmo.c "$(RELSYSDIR)/c_src"
	$(INSTALL_DATA) $(IDL_FILES) $(H_FILES) "$(RELSYSDIR)/include"
	$(INSTALL_DATA) $(LIBRARY) "$(RELSYSDIR)/priv/lib"
ifeq ($(IS_OTP_INSTALL),yes)
	$(INSTALL_DIR) "$(RELEASE_PATH)/usr/include"
	$(INSTALL_DIR) "$(RELEASE_PATH)/usr/lib"
	$(INSTALL_DATA) $(IDL_FILES) $(H_FILES) "$(RELEASE_PATH)/usr/include"
	$(INSTALL_DATA) $(LIBRARY) "$(RELEASE_PATH)/usr/lib"
endif

release_docs_spec:






