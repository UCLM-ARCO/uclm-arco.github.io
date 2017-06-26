# -*- mode: makefile-gmake; coding: utf-8 -*-

THEME ?= default

all:
	hugo --theme=$(THEME)

run-preview-server:
	hugo server --bind "0.0.0.0" --theme=$(THEME)

install-deps:
	sudo apt-get install hugo

.PHONY: clean
clean:
	$(RM) -frv \
	../css ../images ../index.html ../index.xml ../sitemap.xml
