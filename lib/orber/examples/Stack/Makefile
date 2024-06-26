#
# %CopyrightBegin%
# 
# Copyright Ericsson AB 1997-2024. All Rights Reserved.
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

EBIN= ../../ebin

include $(ERL_TOP)/make/$(TARGET)/otp.mk

CORBA_TOP=../../../..

# ----------------------------------------------------
# Application version
# ----------------------------------------------------
include $(CORBA_TOP)/vsn.mk
VSN=$(CORBA_VSN)

# ----------------------------------------------------
# Release directory specification
# ----------------------------------------------------
RELSYSDIR = $(RELEASE_PATH)/lib/orber-$(VSN)

# ----------------------------------------------------
# Target Specs
# ----------------------------------------------------

IDL_FILES = \
	stack.idl \
	InitialReferences.idl

GEN_ERL_MODULES =  \
	oe_stack \
	StackModule_Stack \
	StackModule_StackFactory \
	StackModule_EmptyStack

MODULES= \
	StackModule_Stack_impl \
	StackModule_StackFactory_impl \
	stack_factory \
	stack_client

GEN_HRL_FILES = \
	oe_stack.hrl \
	StackModule.hrl \
	StackModule_Stack.hrl \
	StackModule_StackFactory.hrl

HRL_FILES= 

ERL_FILES= $(MODULES:%=%.erl)

GEN_FILES = $(GEN_ERL_MODULES:%=%.erl) $(GEN_HRL_FILES)

JAVA_CLASSES = \
	StackClient

JAVA_FILES= $(JAVA_CLASSES:%=%.java)
CLASS_FILES= $(JAVA_CLASSES:%=%.class)

TARGET_FILES = \
	$(GEN_ERL_MODULES:%=$(EBIN)/%.$(EMULATOR)) \
	$(MODULES:%=$(EBIN)/%.$(EMULATOR))

TEST_TARGET_FILES = 

CPP_FILES = StackClient.cc

# ----------------------------------------------------
# FLAGS
# ----------------------------------------------------
ERL_IDL_FLAGS += -pa $(CORBA_TOP)/lib/orber/ebin -pa $(CORBA_TOP)/lib/ic/ebin
# The -pa option is just used temporary until erlc can handle 
# includes from other directories than ../include .
ERL_COMPILE_FLAGS += \
	$(ERL_IDL_FLAGS) \
	+nowarn_missing_doc +nowarn_missing_spec_documented \
	-pa $(CORBA_TOP)/lib/orber -I$(CORBA_TOP)/lib/orber

# ----------------------------------------------------
# Targets
# ----------------------------------------------------
debug opt: $(TARGET_FILES) 

clean:
	rm -f $(TARGET_FILES) $(GEN_ERL_MODULES:%=%.erl) $(GEN_HRL_FILES) $(CLASS_FILES) IDL-GENERATED
	rm -f errs core *~

docs:

test: $(TEST_TARGET_FILES)

IDL-GENERATED: stack.idl
	$(gen_verbose)erlc $(ERL_IDL_FLAGS) stack.idl
	$(V_at)>IDL-GENERATED

$(GEN_FILES): IDL-GENERATED

$(TARGET_FILES): IDL-GENERATED

# ----------------------------------------------------
# Release Target
# ---------------------------------------------------- 
include $(ERL_TOP)/make/otp_release_targets.mk


release_spec: opt
	$(INSTALL_DIR) "$(RELSYSDIR)/examples/Stack"
	$(INSTALL_DATA) $(ERL_FILES) $(JAVA_FILES) $(CPP_FILES) $(IDL_FILES) "$(RELSYSDIR)/examples/Stack"


release_docs_spec:


