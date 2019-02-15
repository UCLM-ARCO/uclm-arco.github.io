# -*- mode: makefile-gmake; coding: utf-8 -*-

THEME ?= default

all:
	hugo -v --theme=$(THEME)

run-preview-server:
	hugo server -v --bind "0.0.0.0" --theme=$(THEME)

run-preview-server-with-drafts:
	hugo server -v -D --bind "0.0.0.0" --theme=$(THEME)
