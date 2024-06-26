#
# %CopyrightBegin%
# 
# Copyright Ericsson AB 2000-2027. All Rights Reserved.
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
RELSYSDIR = $(RELEASE_PATH)/lib/cosTime-$(VSN)

EXTERNAL_INC_PATH = ../include

# ----------------------------------------------------
# Target Specs
# ----------------------------------------------------

MODULES = \
	cosTime \
	CosTime_TIO_impl \
	CosTime_TimeService_impl \
	CosTime_UTO_impl \
	CosTimerEvent_TimerEventHandler_impl \
	CosTimerEvent_TimerEventService_impl \

ERL_FILES = $(MODULES:%=%.erl) 
HRL_FILES = \
	cosTimeApp.hrl \

GEN_TIMEBASE_ERL_FILES =  \
	oe_TimeBase.erl \
	TimeBase_IntervalT.erl \
	TimeBase_UtcT.erl

GEN_COSTIME_ERL_FILES =  \
	oe_CosTime.erl \
	CosTime_TIO.erl \
	CosTime_TimeService.erl \
	CosTime_TimeUnavailable.erl \
	CosTime_UTO.erl \

GEN_COSTIMEREVENT_ERL_FILES =  \
	oe_CosTimerEvent.erl \
	CosTimerEvent_TimerEventHandler.erl \
	CosTimerEvent_TimerEventService.erl \
	CosTimerEvent_TimerEventT.erl \

GEN_TIMEBASE_HRL_FILES =  \
	oe_TimeBase.hrl \
	TimeBase.hrl \

EXTERNAL_TIMEBASE_HRL_FILES = $(GEN_TIMEBASE_HRL_FILES:%=$(EXTERNAL_INC_PATH)/%)

GEN_COSTIME_HRL_FILES =  \
	oe_CosTime.hrl \
	CosTime.hrl \
	CosTime_TIO.hrl \
	CosTime_TimeService.hrl \
	CosTime_UTO.hrl

EXTERNAL_COSTIME_HRL_FILES = $(GEN_COSTIME_HRL_FILES:%=$(EXTERNAL_INC_PATH)/%)

GEN_COSTIMEREVENT_HRL_FILES =  \
	oe_CosTimerEvent.hrl \
	CosTimerEvent.hrl \
	CosTimerEvent_TimerEventHandler.hrl \
	CosTimerEvent_TimerEventService.hrl \

EXTERNAL_COSTIMEREVENT_HRL_FILES = $(GEN_COSTIMEREVENT_HRL_FILES:%=$(EXTERNAL_INC_PATH)/%)


GEN_ERL_FILES = \
	$(GEN_TIMEBASE_ERL_FILES) \
	$(GEN_COSTIME_ERL_FILES) \
	$(GEN_COSTIMEREVENT_ERL_FILES)

GEN_HRL_FILES = \
	$(EXTERNAL_TIMEBASE_HRL_FILES) \
	$(EXTERNAL_COSTIME_HRL_FILES) \
	$(EXTERNAL_COSTIMEREVENT_HRL_FILES) \

GEN_FILES =  \
	$(GEN_HRL_FILES) \
	$(GEN_ERL_FILES)

TARGET_FILES = \
	$(GEN_TIMEBASE_ERL_FILES:%.erl=$(EBIN)/%.$(EMULATOR)) \
	$(GEN_COSTIME_ERL_FILES:%.erl=$(EBIN)/%.$(EMULATOR)) \
	$(GEN_COSTIMEREVENT_ERL_FILES:%.erl=$(EBIN)/%.$(EMULATOR)) \
	$(MODULES:%=$(EBIN)/%.$(EMULATOR)) 

IDL_FILES = \
	TimeBase.idl \
	CosTime.idl \
	CosTimerEvent.idl

APPUP_FILE = cosTime.appup
APPUP_SRC = $(APPUP_FILE).src
APPUP_TARGET = $(EBIN)/$(APPUP_FILE)

APP_FILE = cosTime.app
APP_SRC = $(APP_FILE).src
APP_TARGET = $(EBIN)/$(APP_FILE)

# ----------------------------------------------------
# FLAGS
# ----------------------------------------------------
ERL_IDL_FLAGS += -pa $(CORBA_TOP)/lib/cosTime/ebin \
	-pa $(CORBA_TOP)/lib/ic/ebin\
	-pa $(CORBA_TOP)/lib/orber/ebin \
	-I$(CORBA_TOP)/lib/cosEvent/src

# The -pa option is just used temporary until erlc can handle 
# includes from other directories than ../include .
ERL_COMPILE_FLAGS += \
	$(ERL_IDL_FLAGS) \
	-pa $(CORBA_TOP)/lib/orber/include \
	-pa $(CORBA_TOP)/lib/cosTime/include \
	-I$(CORBA_TOP)/lib/cosTime/include \
	-I$(CORBA_TOP)/lib/orber/include \
	+nowarn_missing_doc +nowarn_missing_spec_documented \
	+'{parse_transform,sys_pre_attributes}' \
	+'{attribute,insert,app_vsn,"cosTime_$(VSN)"}' 


# ----------------------------------------------------
# Targets
# ----------------------------------------------------
opt: $(TARGET_FILES) $(APP_TARGET) $(APPUP_TARGET)

debug:
	@${MAKE} TYPE=debug opt

cleanb:
	rm -f $(TARGET_FILES) $(APP_TARGET) $(APPUP_TARGET)
	rm -f errs core *~

clean:
	rm -f $(TARGET_FILES) $(GEN_FILES) $(APP_TARGET) $(APPUP_TARGET) IDL-GENERATED
	rm -f errs core *~

$(APP_TARGET): $(APP_SRC) $(CORBA_TOP)/vsn.mk
	$(vsn_verbose)sed -e 's;%VSN%;$(VSN);' $< > $@

$(APPUP_TARGET): $(APPUP_SRC) $(CORBA_TOP)/vsn.mk
	$(vsn_verbose)sed -e 's;%VSN%;$(VSN);' $< > $@

docs:

# ----------------------------------------------------
# Special Build Targets
# ----------------------------------------------------
IDL-GENERATED: TimeBase.idl CosTime.idl CosTimerEvent.idl
	$(gen_verbose)erlc $(ERL_IDL_FLAGS) TimeBase.idl
	$(V_at)mv $(GEN_TIMEBASE_HRL_FILES) $(EXTERNAL_INC_PATH)
	$(V_at)erlc $(ERL_IDL_FLAGS) +'{cfgfile,"CosTime.cfg"}' CosTime.idl
	$(V_at)mv $(GEN_COSTIME_HRL_FILES) $(EXTERNAL_INC_PATH)
	$(V_at)erlc $(ERL_IDL_FLAGS) +'{cfgfile,"CosTimerEvent.cfg"}' CosTimerEvent.idl
	$(V_at)mv $(GEN_COSTIMEREVENT_HRL_FILES) $(EXTERNAL_INC_PATH)
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
	$(INSTALL_DATA) $(GEN_FILES) $(IDL_FILES) "$(RELSYSDIR)/src"
	$(INSTALL_DATA) $(ERL_FILES) $(HRL_FILES) $(GEN_ERL_FILES) $(IDL_FILES) "$(RELSYSDIR)/src"
	$(INSTALL_DIR) "$(RELSYSDIR)/include"
	$(INSTALL_DATA) $(GEN_HRL_FILES) "$(RELSYSDIR)/include"

release_docs_spec:
