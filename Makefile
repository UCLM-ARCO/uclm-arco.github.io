# -*- mode: makefile-gmake; coding: utf-8 -*-

HUGO_SITE = hugo-site
PROJECTS  = arco101 citisim hesperia idm icec symbiot

all:

hugo-sync:
	-@for prj in $(PROJECTS); do \
		make sync-prj PRJ=$$prj; \
	done

hugo-clean:
	@for dir in content layouts static; do \
		$(RM) -r $(HUGO_SITE)/$$dir; \
		mkdir -p $(HUGO_SITE)/$$dir; \
	done

sync-prj:
	@for dir in content layouts static; do \
		if [ -d "$(PRJ)/$$dir" ]; then \
			rsync -avz "$(PRJ)/$$dir/" "$(HUGO_SITE)/$$dir/$(PRJ)"; \
		fi; \
	done

keep-sync:
	while inotifywait -r -e modify,create,delete $(PROJECTS); do \
		make hugo-sync; \
	done

