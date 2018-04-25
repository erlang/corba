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

# ----------------------------------------------------
# Common Macros
# ----------------------------------------------------

SUB_DIRECTORIES = lib/orber lib/ic lib/cosEvent \
	lib/cosTime lib/cosNotification lib/cosEventDomain \
	lib/cosProperty lib/cosFileTransfer lib/cosTransactions

SPECIAL_TARGETS = 

# ----------------------------------------------------
# Default Subdir Targets
# ----------------------------------------------------
include $(ERL_TOP)/make/otp_subdir.mk

# ----------------------------------------------------
# Configure Targets
# ----------------------------------------------------
autoconf:
	@if [ -d "autom4te.cache" ]; then \
		echo "=== cleaning autom4te.cache"; \
		rm -f autom4te.cache/*; \
	fi; \
	echo "=== running autoconf"; \
	autoconf; \
	chmod 755 configure; \
	chdr=`cat "configure.in" | sed -n "s|.*\(AC_CONFIG_HEADER\).*|\1|p"`;\
	if [ "$chdr" = "AC_CONFIG_HEADER" ]; then \
		echo "=== running autoheader"; \
		autoheader; \
	fi

configure: autoconf
	./configure
