#
# %CopyrightBegin%
#
# Copyright Ericsson AB 1997-2017. All Rights Reserved.
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

CORBA_TOP=..

# ----------------------------------------------------
# Application version
# ----------------------------------------------------
include $(CORBA_TOP)/vsn.mk
VSN=$(CORBA_VSN)
#APPLICATION=

# ----------------------------------------------------
# Release directory specification
# ----------------------------------------------------
RELDOC_DIR = "$(RELEASE_PATH)/doc"

# ----------------------------------------------------
# Target Specs
# ----------------------------------------------------
INFO_FILES = ../README.md ../COPYRIGHT

TOPDOCDIR=.

# ----------------------------------------------------

INDEX_FILE = index.html

CSS_PATH=$(ERL_TOP)/lib/erl_docgen/priv/css

CSS_FILES= \
	$(CSS_PATH)/otp_doc.css \
	$(CSS_PATH)/highlight.css

JS_PATH=$(ERL_TOP)/lib/erl_docgen/priv/js

HIGHLIGHT_FILES= \
	$(JS_PATH)/highlight.js \
	$(JS_PATH)/highlight.pack.js

FLIPMENU_FILES= \
	$(JS_PATH)/flipmenu/flipmenu.js \
	$(JS_PATH)/flipmenu/flip_closed.gif \
	$(JS_PATH)/flipmenu/flip_open.gif \
	$(JS_PATH)/flipmenu/flip_static.gif


IMAGE_FILES=$(ERL_TOP)/lib/erl_docgen/priv/images/erlang-logo.png
# ----------------------------------------------------
# FLAGS
# ----------------------------------------------------
XML_FLAGS +=
DVIPS_FLAGS +=

# ----------------------------------------------------
# Targets
# ----------------------------------------------------
$(HTMLDIR)/%.gif: %.gif
	$(INSTALL_DATA) $< $@

docs: pdf html man

pdf: 

html: gifs $(INDEX_FILE)

$(INDEX_FILE): $(INDEX_FILE).src
	$(vsn_verbose)sed 's/#corba_base_vsn#/$(VSN)/g' $< > $@

clean clean_docs:
	$(RM) $(INDEX_FILE)
man:

gifs:

debug opt:

# ----------------------------------------------------
# Release Target
# ----------------------------------------------------
include $(ERL_TOP)/make/otp_release_targets.mk

release_docs_spec: docs
	$(INSTALL_DIR) "$(RELDOC_DIR)"
	$(INSTALL_DATA) $(INDEX_FILE) "$(RELDOC_DIR)"
	$(INSTALL_DATA) $(IMAGE_FILES) "$(RELDOC_DIR)"
	$(INSTALL_DATA) $(CSS_FILES) "$(RELDOC_DIR)"
	$(INSTALL_DIR) "$(RELDOC_DIR)/js/flipmenu"
	$(INSTALL_DATA) $(HIGHLIGHT_FILES) "$(RELDOC_DIR)/js"
	$(INSTALL_DATA) $(FLIPMENU_FILES) "$(RELDOC_DIR)/js/flipmenu"
	$(INSTALL_DATA) $(INFO_FILES) "$(RELEASE_PATH)"

release_spec:
