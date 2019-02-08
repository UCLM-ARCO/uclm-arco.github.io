# -*- mode: makefile-gmake; coding: utf-8 -*-

THEME ?= default

all:
	hugo --theme=$(THEME)

run-preview-server:
	hugo server --bind "0.0.0.0" --theme=$(THEME) --buildDrafts

install-deps:
	sudo apt-get install hugo


.PHONY: clean
clean:
	for f in ../*; do \
	  if [ "$$f" != "../src" -a "$$f" != "../README" ]; then \
	    rm -rvf "$$f"; \
	  fi; \
	done
