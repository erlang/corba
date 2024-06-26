#
# %CopyrightBegin%
# 
# Copyright Ericsson AB 1999-2024. All Rights Reserved.
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
# ``Licensed under the Apache License, Version 2.0 (the "License");
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
# The Initial Developer of the Original Code is Ericsson Utvecklings AB.
# Portions created by Ericsson are Copyright 1999, Ericsson Utvecklings
# AB. All Rights Reserved.''
# 
#     $Id$
#
include $(ERL_TOP)/make/target.mk

EBIN= ./

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
RELSYSDIR = $(RELEASE_PATH)/lib/ic-$(VSN)

# ----------------------------------------------------
# Target Specs
# ----------------------------------------------------

IDL_FILES = \
	ex.idl 

GEN_ERL_MODULES =  \
	oe_ex \
	m_i \
	m_NotAnInteger

MODULES= \
	m_i_impl \
	tracer

GEN_HRL_FILES =  \
	oe_ex.hrl \
	m.hrl \
	m_i.hrl

HRL_FILES = 
TXT_FILES = ReadMe.txt

ERL_FILES= $(MODULES:%=%.erl)


TARGET_FILES = \
	$(GEN_ERL_MODULES:%=$(EBIN)/%.$(EMULATOR)) \
	$(MODULES:%=$(EBIN)/%.$(EMULATOR))

# ----------------------------------------------------
# FLAGS
# ----------------------------------------------------
ERL_LOCAL_FLAGS += -pa $(CORBA_TOP)/lib/orber/ebin -pa $(CORBA_TOP)/lib/ic/ebin
ERL_COMPILE_FLAGS += \
	$(ERL_LOCAL_FLAGS) \
	+nowarn_missing_doc +nowarn_missing_spec_documented \
	-pa $(CORBA_TOP)/lib/orber -I$(CORBA_TOP)/lib/orber
YRL_FLAGS = 


# ----------------------------------------------------
# Targets
# ----------------------------------------------------
debug opt: $(TARGET_FILES) 

clean:
	rm -f $(TARGET_FILES) $(GEN_ERL_MODULES:%=%.erl) $(GEN_HRL_FILES) $(CLASS_FILES) IDL-GENERATED
	rm -f errs core *~

docs:

test: $(TEST_TARGET_FILES)


IDL-GENERATED: ex.idl
	$(gen_verbose)erlc $(ERL_LOCAL_FLAGS) +'{precond,{tracer,pre}}' \
		+'{{postcond,"m::i::f"},{tracer,post}}' ex.idl
	$(V_at)>IDL-GENERATED

$(GEN_ERL_MODULES:%=%.erl) $(GEN_HRL_FILES): IDL-GENERATED

$(TARGET_FILES): IDL-GENERATED

# ----------------------------------------------------
# Release Target
# ---------------------------------------------------- 
include $(ERL_TOP)/make/otp_release_targets.mk


release_spec: opt
	$(INSTALL_DIR) "$(RELSYSDIR)/examples/pre_post_condition"
	$(INSTALL_DATA) $(ERL_FILES) $(IDL_FILES) $(TXT_FILES) "$(RELSYSDIR)/examples/pre_post_condition"


release_docs_spec:


