#
# %CopyrightBegin%
# 
# Copyright Ericsson AB 1997-2016. All Rights Reserved.
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

OBJDIR = ../priv/obj/$(TARGET)

# ----------------------------------------------------
# Application version
# ----------------------------------------------------
include ../../../../vsn.mk
VSN=$(CORBA_VSN)

# ----------------------------------------------------
# Release directory specification
# ----------------------------------------------------
RELSYSDIR = $(RELEASE_PATH)/lib/orber-$(VSN)

# ----------------------------------------------------
# Target Specs
# ----------------------------------------------------
CC_FILES = \
	InitialReference.cc

HH_FILES = \
	InitialReference.hh

ALL_CFLAGS = $(BASIC_CFLAGS)

# ----------------------------------------------------
# Targets
# ----------------------------------------------------

debug opt: $(OBJDIR) orber

ifeq ($(findstring win32,$(TARGET)),win32)
orber: 
	$(V_colon)echo "Nothing to build on NT"
else
orber: 
	$(V_colon)echo "Nothing to build"
endif

clean:

docs:

# ----------------------------------------------------
# Special Build Targets
# ----------------------------------------------------

$(OBJDIR):
	-mkdir -p $(OBJDIR)

$(OBJDIR)/%.o: %.c
	$(V_CC) -c -o $@ $(ALL_CFLAGS) $<

# ----------------------------------------------------
# Release Target
# ---------------------------------------------------- 
include $(ERL_TOP)/make/otp_release_targets.mk

ifeq ($(findstring win32,$(TARGET)),win32)
release_spec: opt
	$(INSTALL_DIR) "$(RELSYSDIR)/priv/src"
	$(INSTALL_DIR) "$(RELSYSDIR)/priv/include"
	$(INSTALL_PROGRAM) $(CC_FILES) "$(RELSYSDIR)/priv/src"
	$(INSTALL_PROGRAM) $(HH_FILES) "$(RELSYSDIR)/priv/include"
else
release_spec: opt
	$(INSTALL_DIR) "$(RELSYSDIR)/priv/src"
	$(INSTALL_DIR) "$(RELSYSDIR)/priv/include"
	$(INSTALL_DATA) $(CC_FILES) "$(RELSYSDIR)/priv/src"
	$(INSTALL_DATA) $(HH_FILES) "$(RELSYSDIR)/priv/include"
endif


release_docs_spec:
