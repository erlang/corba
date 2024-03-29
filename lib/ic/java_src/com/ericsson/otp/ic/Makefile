#
# %CopyrightBegin%
# 
# Copyright Ericsson AB 1999-2016. All Rights Reserved.
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

CORBA_TOP=../../../../../../..

JAVA_DEST_ROOT = $(CORBA_TOP)/lib/ic/priv/
JAVA_SRC_ROOT = $(CORBA_TOP)/lib/ic/java_src/
JAVA_CLASS_SUBDIR = com/ericsson/otp/ic/

JAVA_INCL_ROOT := $(ERL_TOP)/lib/jinterface/priv

include $(ERL_TOP)/make/$(TARGET)/otp.mk

# ----------------------------------------------------
# Application version
# ----------------------------------------------------
include  $(CORBA_TOP)/vsn.mk

# ----------------------------------------------------
# Release directory specification
# ----------------------------------------------------
RELSYSDIR = $(RELEASE_PATH)/lib/ic-$(CORBA_VSN)

#
# JAVA macros
#
JAVA_CLASSES = \
	Holder \
	BooleanHolder \
	ByteHolder \
	CharHolder \
	DoubleHolder \
	FloatHolder \
	IntHolder \
	LongHolder \
	ShortHolder \
	StringHolder \
	Environment \
	Any \
	AnyHelper \
	AnyHolder \
	TypeCode \
	TCKind \
	Pid \
	PidHolder \
	PidHelper \
	Ref \
	RefHolder \
	RefHelper \
	Port \
	PortHolder \
	PortHelper \
	Term \
	TermHolder \
	TermHelper

TARGET_FILES= $(JAVA_CLASSES:%=$(JAVA_DEST_ROOT)$(JAVA_CLASS_SUBDIR)%.class)
JAVA_FILES= $(JAVA_CLASSES:%=%.java)

JARFILE= ic.jar

# ----------------------------------------------------
# Programs and Flags
# ----------------------------------------------------
CLASSPATH = $(JAVA_SRC_ROOT):$(JAVA_INCL_ROOT)/OtpErlang.jar

JAR= jar

JAVADOCFLAGS=-d $(DOCDIR)
JAVAFLAGS=-d $(JAVA_DEST_ROOT) 
JARFLAGS= -cf
ifneq ($(V),0)
JARFLAGS= -cfv
endif

JAVA_OPTIONS = 

# ----------------------------------------------------
# Make Rules
# ----------------------------------------------------

debug opt: $(JAVA_DEST_ROOT)$(JARFILE)

$(JAVA_DEST_ROOT)$(JARFILE): $(TARGET_FILES)
	@(cd $(JAVA_DEST_ROOT) ; $(JAR) $(JARFLAGS) $(JARFILE) $(JAVA_CLASS_SUBDIR))

clean:
	rm -f $(TARGET_FILES) *~

docs:

# ----------------------------------------------------
# Release Targets
# ----------------------------------------------------
include $(ERL_TOP)/make/otp_release_targets.mk

release_spec: opt
	$(INSTALL_DIR) "$(RELSYSDIR)/java_src/com/ericsson/otp/ic"
	$(INSTALL_DATA) $(JAVA_FILES) "$(RELSYSDIR)/java_src/com/ericsson/otp/ic"
	$(INSTALL_DIR) "$(RELSYSDIR)/priv"
	$(INSTALL_DATA) $(JAVA_DEST_ROOT)$(JARFILE) "$(RELSYSDIR)/priv"

release_docs_spec:

