#
# %CopyrightBegin%
# 
# Copyright Ericsson AB 2000-2024. All Rights Reserved.
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

CORBA_TOP=../../..

# ----------------------------------------------------
# Application version
# ----------------------------------------------------
include $(CORBA_TOP)/vsn.mk
VSN=$(CORBA_VSN)

# ----------------------------------------------------
# Release directory specification
# ----------------------------------------------------
RELSYSDIR = $(RELEASE_PATH)/cosTime_test

# ----------------------------------------------------
# Target Specs
# ----------------------------------------------------
TEST_SPEC_FILE = cosTime.spec
COVER_FILE = cosTime.cover


IDL_FILES = 

IDLOUTDIR = idl_output

MODULES =  \
	time_SUITE \
	generated_SUITE

GEN_MODULES = \

GEN_HRL_FILES = \

ERL_FILES = $(MODULES:%=%.erl)

HRL_FILES =

GEN_FILES = \
	$(GEN_HRL_FILES:%=$(IDLOUTDIR)/%) \
	$(GEN_MODULES:%=$(IDLOUTDIR)/%.erl) 

GEN_TARGET_FILES = $(GEN_MODULES:%=$(IDLOUTDIR)/%.$(EMULATOR))

SUITE_TARGET_FILES = $(MODULES:%=%.$(EMULATOR))

TARGET_FILES = \
	$(GEN_TARGET_FILES) \
	$(SUITE_TARGET_FILES)


# ----------------------------------------------------
# PROGRAMS
# ----------------------------------------------------
LOCAL_CLASSPATH = $(CORBA_TOP)lib/cosTime/priv:$(CORBA_TOP)lib/cosTime/test
# ----------------------------------------------------
# FLAGS
# ----------------------------------------------------
ERL_IDL_FLAGS += -pa $(CORBA_TOP)/lib/cosTime/ebin \
	-pa $(CORBA_TOP)/lib/cosTime/src \
	-pa $(CORBA_TOP)/lib/cosTime/include \
	-pa $(CORBA_TOP)/lib/cosNotification/ebin \
	-pa $(CORBA_TOP)/lib/orber/ebin \
	-pa $(CORBA_TOP)/lib/ic/ebin

ERL_COMPILE_FLAGS +=  \
	$(ERL_IDL_FLAGS) \
	-pa $(CORBA_TOP)/lib/orber/include \
	-pa $(CORBA_TOP)/lib/cosNotification/include \
	-pa $(CORBA_TOP)/lib/cosTime/ebin \
	-pa $(CORBA_TOP)/lib/cosTime/include \
	-pa $(CORBA_TOP)/lib/cosTime/test/idl_output \
	+nowarn_missing_doc +nowarn_missing_spec_documented \
	-I$(CORBA_TOP)/lib/orber/include \
	-I$(CORBA_TOP)/lib/cosNotification/include \
	-I$(CORBA_TOP)/lib/cosTime/src \
	-I$(CORBA_TOP)/lib/cosTime/include \
	-I$(CORBA_TOP)/lib/cosTime \
	-I$(CORBA_TOP)/lib/cosTime/test/$(IDLOUTDIR)

# ----------------------------------------------------
# Targets
# ----------------------------------------------------


tests debug opt: $(TARGET_FILES)

clean:
	rm -f idl_output/*
	rm -f $(TARGET_FILES) 
	rm -f errs core *~

docs:

# ----------------------------------------------------
# Special Targets
# ----------------------------------------------------

# ----------------------------------------------------
# Release Targets
# ---------------------------------------------------- 
# We don't copy generated intermediate erlang and hrl files

include $(ERL_TOP)/make/otp_release_targets.mk

release_spec: 

release_docs_spec:

release_tests_spec: tests
	$(INSTALL_DIR) "$(RELSYSDIR)"
	$(INSTALL_DATA) $(IDL_FILES)  $(TEST_SPEC_FILE) \
		$(COVER_FILE) $(ERL_FILES) "$(RELSYSDIR)"
	$(INSTALL_DATA) $(SUITE_TARGET_FILES) "$(RELSYSDIR)"
#	$(INSTALL_DIR) "$(RELSYSDIR)/$(IDLOUTDIR)"
#	$(INSTALL_DATA) $(GEN_TARGET_FILES) $(GEN_FILES) \
#		"$(RELSYSDIR)/$(IDLOUTDIR)"

