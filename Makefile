# -*- mode: makefile-gmake; coding: utf-8 -*-

SUBDIRS = home idm/src icec/src

all:     RULE = all
clean:   RULE = clean

all clean: subdirs

.PHONY: subdirs $(SUBDIRS)
subdirs: $(SUBDIRS)
$(SUBDIRS):
	$(MAKE) -C $@ $(RULE)
