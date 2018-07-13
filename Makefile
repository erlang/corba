#
# %CopyrightBegin%
# 
# Copyright Ericsson AB 1997-2018. All Rights Reserved.
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

ifeq ($(ERL_TOP),)
$(error ERL_TOP needs to be set)
endif

OTP_CONF := $(shell $(MAKE) -f $(ERL_TOP)/Makefile erlang_inst_libdir_configured 1>/dev/null 2>/dev/null; echo $$?)
ifneq ($(OTP_CONF),0)
$(error Either there is no OTP source tree in $(ERL_TOP), the OTP version is too old, or the OTP source tree has not been configured)
endif

INST_DIR := $(shell $(MAKE) -f $(ERL_TOP)/Makefile erlang_inst_libdir_configured)

TARGET := $(shell $(MAKE) -f $(ERL_TOP)/Makefile target_configured)
include $(ERL_TOP)/make/target.mk
export TARGET
include $(ERL_TOP)/make/$(TARGET)/otp.mk

BOOTSTRAP_ROOT = $(ERL_TOP)
ifeq ($(TARGET),win32)
BOOT_PREFIX=$(WIN32_WRAPPER_PATH):$(BOOTSTRAP_ROOT)/bootstrap/bin:
else
BOOT_PREFIX=$(BOOTSTRAP_ROOT)/bootstrap/bin:
endif

ifneq ($(JAVA),)
HAVE_JAVAC := $(shell rm -f lib/ic/java_src/SKIP; echo true)
else
HAVE_JAVAC := $(shell echo "No Java compiler found" > lib/ic/java_src/SKIP; echo false)
endif

ifneq ($(RELEASE_ROOT),)
TESTROOT=$(RELEASE_ROOT)
endif

# ----------------------------------------------------
# Common Macros
# ----------------------------------------------------

SUB_DIRECTORIES = lib/ic lib/orber lib/cosEvent \
	lib/cosTime lib/cosNotification lib/cosEventDomain \
	lib/cosProperty lib/cosFileTransfer lib/cosTransactions doc

SPECIAL_TARGETS = 

TEST_SUB_DIRECTORIES = \
	$(wildcard lib/*/test)

# ----------------------------------------------------
# Default Subdir Targets
# ----------------------------------------------------
.PHONY: debug opt release docs release_docs \
	clean depend valgrind xmllint

release: opt

release_docs: docs

opt debug release docs release_docs clean depend xmllint:
	@set -e ;							\
	if test $(CROSS_COMPILING) = yes; then				\
	    if test "$@" = opt || test "$@" = debug; then		\
		$(MAKE) -f $(ERL_TOP)/Makefile cross_check_erl;		\
	    fi;								\
	fi;								\
	if test "$(TESTROOT)" = "" 					\
	   || test ! -f "$(TESTROOT)/usr/include/erl_nif.h"; then	\
	    is_otp_install=no;						\
	else								\
	    if test ! -f "$(TESTROOT)/releases/$(SYSTEM_VSN)/OTP_VERSION"; then \
		echo "ERROR: Trying to install into an OTP" 1>&2;	\
		echo "       installation of different release" 1>&2;	\
		echo "       than the OTP release used to build" 1>&2;	\
		echo "       with." 1>&2;				\
		exit 1;							\
	    fi;								\
	    is_otp_install=yes;						\
	fi;								\
	for d in $(SUB_DIRECTORIES); do					\
	    if test -f $$d/SKIP ; then					\
		echo "=== Skipping subdir $$d, reason:" ;		\
		cat $$d/SKIP ;						\
		echo "===" ;						\
	    else							\
		if test ! -d $$d ; then					\
		    echo "=== Skipping subdir $$d, it is missing" ;	\
		else							\
		    (cd $$d &&						\
			PATH="$(BOOT_PREFIX)$${PATH}" $(MAKE) $@	\
			TARGET=$(TARGET) ERL_TOP=$(ERL_TOP)		\
			TESTROOT=$(TESTROOT) 				\
			IS_OTP_INSTALL=$$is_otp_install) || exit $$? ;	\
		fi ;							\
	    fi ;							\
	done ;

install install-docs: TESTROOT=$(DESTDIR)$(shell $(MAKE) -f $(ERL_TOP)/Makefile erlang_inst_libdir_configured)

install: release

install-docs: release_docs

.PHONY: tests release_tests

tests release_tests:
	@set -e ;							\
	for d in $(TEST_SUB_DIRECTORIES); do				\
	    (cd $$d && $(MAKE) $@) || exit $$? ;			\
	done ;
