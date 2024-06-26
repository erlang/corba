#
# %CopyrightBegin%
# 
# Copyright Ericsson AB 1999-2027. All Rights Reserved.
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
EBIN=../ebin

ifeq ($(TYPE),debug)
ERL_COMPILE_FLAGS += -Ddebug -W
endif

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
RELSYSDIR = $(RELEASE_PATH)/lib/cosTransactions-$(VSN)
# ----------------------------------------------------
# Target Specs
# ----------------------------------------------------

MODULES = \
	ETraP_Common \
	etrap_logmgr \
	ETraP_Server_impl \
	CosTransactions_Terminator_impl \
	CosTransactions_TransactionFactory_impl \
	cosTransactions

ERL_FILES = $(MODULES:%=%.erl) 
HRL_FILES = \
	ETraP_Common.hrl

GEN_ERL_FILES =  \
	oe_CosTransactions.erl \
	CosTransactions_Control.erl \
	CosTransactions_Coordinator.erl \
	CosTransactions_HeuristicCommit.erl \
	CosTransactions_HeuristicHazard.erl \
	CosTransactions_HeuristicMixed.erl \
	CosTransactions_HeuristicRollback.erl \
	CosTransactions_Inactive.erl \
	CosTransactions_InvalidControl.erl \
	CosTransactions_NoTransaction.erl \
	CosTransactions_NotPrepared.erl \
	CosTransactions_NotSubtransaction.erl \
	CosTransactions_RecoveryCoordinator.erl \
	CosTransactions_Resource.erl \
	CosTransactions_SubtransactionAwareResource.erl \
	CosTransactions_SubtransactionsUnavailable.erl \
	CosTransactions_Terminator.erl \
	CosTransactions_TransactionFactory.erl \
	CosTransactions_Unavailable.erl \
	CosTransactions_SynchronizationUnavailable.erl \
	CosTransactions_TransIdentity.erl \
	CosTransactions_PropagationContext.erl \
	CosTransactions_otid_t.erl \
	CosTransactions_WrongTransaction.erl \
	ETraP_Server.erl
#	CosTransactions_Synchronization.erl \

EXTERNAL_INC_PATH = ../include

GEN_HRL_FILES = \
	oe_CosTransactions.hrl \
	CosTransactions.hrl \
	CosTransactions_Control.hrl \
	CosTransactions_Coordinator.hrl \
	CosTransactions_RecoveryCoordinator.hrl \
	CosTransactions_Resource.hrl \
	CosTransactions_SubtransactionAwareResource.hrl \
	CosTransactions_Terminator.hrl \
	CosTransactions_TransactionFactory.hrl \
	ETraP.hrl \
	ETraP_Server.hrl
#	CosTransactions_Synchronization.hrl \

EXTERNAL_GEN_HRL_FILES = $(GEN_HRL_FILES:%=$(EXTERNAL_INC_PATH)/%)

GEN_FILES = $(GEN_ERL_FILES) $(EXTERNAL_GEN_HRL_FILES)

TARGET_FILES = \
	$(GEN_ERL_FILES:%.erl=$(EBIN)/%.$(EMULATOR)) \
	$(MODULES:%=$(EBIN)/%.$(EMULATOR)) 

IDL_FILE = \
	CosTransactions.idl

APPUP_FILE = cosTransactions.appup
APPUP_SRC = $(APPUP_FILE).src
APPUP_TARGET = $(EBIN)/$(APPUP_FILE)

APP_FILE = cosTransactions.app
APP_SRC = $(APP_FILE).src
APP_TARGET = $(EBIN)/$(APP_FILE)

# ----------------------------------------------------
# FLAGS
# ----------------------------------------------------
ERL_IDL_FLAGS += -pa $(CORBA_TOP)/lib/cosTransactions/ebin \
	-pa $(CORBA_TOP)/lib/ic/ebin\
	-pa $(CORBA_TOP)/lib/orber/ebin

# The -pa option is just used temporary until erlc can handle 
# includes from other directories than ../include .
ERL_COMPILE_FLAGS += \
	$(ERL_IDL_FLAGS) \
	-I$(CORBA_TOP)/lib/cosTransactions/include \
	-I$(CORBA_TOP)/lib/orber/include \
	+nowarn_missing_doc +nowarn_missing_spec_documented \
	+'{parse_transform,sys_pre_attributes}' \
	+'{attribute,insert,app_vsn,"cosTransactions_$(VSN)"}' 


# ----------------------------------------------------
# Targets
# ----------------------------------------------------
opt: $(TARGET_FILES) $(APP_TARGET) $(APPUP_TARGET)

debug:
	@${MAKE} TYPE=debug

clean:
	rm -f $(TARGET_FILES) $(GEN_FILES) $(APP_TARGET) $(APPUP_TARGET) IDL-GENERATED
	rm -f errs core *~

$(APP_TARGET): $(APP_SRC) $(CORBA_TOP)/vsn.mk
	$(vsn_verbose)sed -e 's;%VSN%;$(VSN);' $(APP_SRC) > $(APP_TARGET)

$(APPUP_TARGET): $(APPUP_SRC) $(CORBA_TOP)/vsn.mk
	$(vsn_verbose)sed -e 's;%VSN%;$(VSN);' $< > $@

docs:

# ----------------------------------------------------
# Special Build Targets
# ----------------------------------------------------
IDL-GENERATED: CosTransactions.idl
	$(gen_verbose)erlc $(ERL_IDL_FLAGS) +'{cfgfile,"CosTransactions.cfg"}' CosTransactions.idl
	$(V_at)mv $(GEN_HRL_FILES) $(EXTERNAL_INC_PATH)
	$(V_at)>IDL-GENERATED

$(GEN_FILES): IDL-GENERATED

$(TARGET_FILES): IDL-GENERATED

# ----------------------------------------------------
# Release Target
# ---------------------------------------------------- 
include $(ERL_TOP)/make/otp_release_targets.mk

release_spec: opt
	$(INSTALL_DIR) "$(RELSYSDIR)/ebin"
	$(INSTALL_DATA) $(TARGET_FILES) $(APP_TARGET) $(APPUP_TARGET) "$(RELSYSDIR)/ebin"
	$(INSTALL_DIR) "$(RELSYSDIR)/src"
	$(INSTALL_DATA) $(ERL_FILES) $(HRL_FILES) $(GEN_ERL_FILES) $(IDL_FILE) "$(RELSYSDIR)/src"
	$(INSTALL_DIR) "$(RELSYSDIR)/include"
	$(INSTALL_DATA) $(EXTERNAL_GEN_HRL_FILES) "$(RELSYSDIR)/include"

release_docs_spec:
