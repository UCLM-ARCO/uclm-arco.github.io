# -*- mode: makefile-gmake; coding: utf-8 -*-

THEME ?= default

all:
	hugo --theme=$(THEME)

run-preview-server:
	hugo server -v --bind "0.0.0.0" --theme=$(THEME)

run-preview-server-with-drafts:
	hugo server -v -D --bind "0.0.0.0" --theme=$(THEME)

install-deps:
	sudo apt-get install hugo

.PHONY: clean
clean:
	$(RM) -frv \
	../css ../images ../webfonts ../js \
	../index.html ../index.xml ../sitemap.xml \
	../categories ../tags \
	../citisim ../arco101 ../hesperia ../idm ../icec ../symbiot
