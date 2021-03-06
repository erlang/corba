#
# %CopyrightBegin%
#
# Copyright Ericsson AB 2000-2018. All Rights Reserved.
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

CORBA_TOP=../../../..
NO_GITHUB_DOC_LINKS=true

# ----------------------------------------------------
# Application version
# ----------------------------------------------------
include $(CORBA_TOP)/vsn.mk
VSN=$(CORBA_VSN)
APPLICATION=cosNotification

# ----------------------------------------------------
# Release directory specification
# ----------------------------------------------------
RELSYSDIR = $(RELEASE_PATH)/lib/$(APPLICATION)-$(VSN)

# ----------------------------------------------------
# Target Specs
# ----------------------------------------------------
XML_APPLICATION_FILES = ref_man.xml
XML_REF3_FILES = \
	cosNotificationApp.xml \
	CosNotifyChannelAdmin_EventChannelFactory.xml \
	CosNotifyChannelAdmin_EventChannel.xml \
	CosNotification.xml \
	CosNotification_QoSAdmin.xml \
	CosNotification_AdminPropertiesAdmin.xml \
	CosNotifyChannelAdmin_ConsumerAdmin.xml \
	CosNotifyChannelAdmin_SupplierAdmin.xml \
	CosNotifyComm_NotifyPublish.xml \
	CosNotifyComm_NotifySubscribe.xml \
	CosNotifyFilter_FilterAdmin.xml \
	CosNotifyFilter_FilterFactory.xml \
	CosNotifyFilter_Filter.xml \
	CosNotifyFilter_MappingFilter.xml \
	CosNotifyChannelAdmin_ProxyConsumer.xml \
	CosNotifyChannelAdmin_ProxySupplier.xml \
	CosNotifyChannelAdmin_ProxyPullConsumer.xml \
	CosNotifyChannelAdmin_ProxyPullSupplier.xml \
	CosNotifyChannelAdmin_ProxyPushConsumer.xml \
	CosNotifyChannelAdmin_ProxyPushSupplier.xml \
	CosNotifyChannelAdmin_SequenceProxyPullConsumer.xml \
	CosNotifyChannelAdmin_SequenceProxyPullSupplier.xml \
	CosNotifyChannelAdmin_SequenceProxyPushConsumer.xml \
	CosNotifyChannelAdmin_SequenceProxyPushSupplier.xml \
	CosNotifyChannelAdmin_StructuredProxyPullConsumer.xml \
	CosNotifyChannelAdmin_StructuredProxyPullSupplier.xml \
	CosNotifyChannelAdmin_StructuredProxyPushConsumer.xml \
	CosNotifyChannelAdmin_StructuredProxyPushSupplier.xml \



XML_PART_FILES = \
	part.xml
XML_CHAPTER_FILES = \
	ch_contents.xml \
	ch_introduction.xml \
	ch_install.xml \
	ch_system.xml \
	ch_BNF.xml \
	ch_QoS.xml \
	ch_example.xml \
	notes.xml

BOOK_FILES = book.xml

XML_FILES = $(BOOK_FILES) $(XML_APPLICATION_FILES) $(XML_REF3_FILES) \
            $(XML_PART_FILES) $(XML_CHAPTER_FILES)

TECHNICAL_DESCR_FILES =

GIF_FILES = \
	eventstructure.gif \
	notificationFlow.gif

PS_FILES =

# ----------------------------------------------------

INTERNAL_HTML_FILES = $(TECHNICAL_DESCR_FILES:%.xml=$(HTMLDIR)/%.html)

HTML_FILES = $(XML_APPLICATION_FILES:%.xml=$(HTMLDIR)/%.html) \
	$(XML_PART_FILES:%.xml=$(HTMLDIR)/%.html)

INFO_FILE = ../../info
EXTRA_FILES = \
	$(DEFAULT_GIF_FILES) \
	$(DEFAULT_HTML_FILES) \
	$(XML_REF3_FILES:%.xml=$(HTMLDIR)/%.html) \
	$(XML_CHAPTER_FILES:%.xml=$(HTMLDIR)/%.html)

MAN3_FILES = $(XML_REF3_FILES:%.xml=$(MAN3DIR)/%.3)

HTML_REF_MAN_FILE = $(HTMLDIR)/index.html

TOP_PDF_FILE = $(PDFDIR)/$(APPLICATION)-$(VSN).pdf

# ----------------------------------------------------
# FLAGS
# ----------------------------------------------------
XML_FLAGS +=
DVIPS_FLAGS +=

# ----------------------------------------------------
# Targets
# ----------------------------------------------------

notes.xml: ../../../../doc/notes_head.xml.src notes_tail.xml.src
	$(gen_verbose)cat $< > $@ && cat notes_tail.xml.src >> $@

$(HTMLDIR)/%.gif: %.gif
	$(INSTALL_DATA) $< $@

docs: notes.xml pdf html man

$(TOP_PDF_FILE): $(XML_FILES)

pdf: $(TOP_PDF_FILE)

html: gifs $(HTML_REF_MAN_FILE)

clean clean_docs:
	rm -rf $(HTMLDIR)/*
	rm -f $(MAN3DIR)/*
	rm -f ../xml/*
	rm -f $(TOP_PDF_FILE) $(TOP_PDF_FILE:%.pdf=%.fo)
	rm -f notes.xml errs core *~

man: $(MAN3_FILES)

gifs: $(GIF_FILES:%=$(HTMLDIR)/%)

debug opt:

# ----------------------------------------------------
# Release Target
# ----------------------------------------------------
include $(ERL_TOP)/make/otp_release_targets.mk

release_docs_spec: docs
	$(INSTALL_DIR) "$(RELSYSDIR)/doc/pdf"
	$(INSTALL_DATA) $(TOP_PDF_FILE) "$(RELSYSDIR)/doc/pdf"
	$(INSTALL_DIR) "$(RELSYSDIR)/doc/html"
	$(INSTALL_DATA) $(HTMLDIR)/* \
		"$(RELSYSDIR)/doc/html"
	$(INSTALL_DATA) $(INFO_FILE) "$(RELSYSDIR)"
	$(INSTALL_DIR) "$(RELEASE_PATH)/man/man3"
	$(INSTALL_DATA) $(MAN3DIR)/* "$(RELEASE_PATH)/man/man3"

release_spec:
