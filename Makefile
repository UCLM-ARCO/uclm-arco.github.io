# -*- mode: makefile-gmake; coding: utf-8 -*-

HUGO_SITE = hugo-site
PROJECTS  = arco101 citisim hesperia idm icec symbiot

all run-preview-server run-preview-server-with-drafts:
	$(MAKE) -C $(HUGO_SITE) $@

sync: hugo-clean hugo-sync keep-sync

hugo-sync:
	-@for prj in $(PROJECTS); do \
		make sync-prj PRJ=$$prj; \
	done

hugo-clean:
	@for dir in content layouts static; do \
		$(RM) -rv $(HUGO_SITE)/$$dir; \
		mkdir -p $(HUGO_SITE)/$$dir; \
	done
	mkdir -p $(HUGO_SITE)/layouts/partials
	cp search/search-index.md $(HUGO_SITE)/content

sync-prj:
	@for dir in content layouts layouts/partials static; do \
		if [ -d "$(PRJ)/$$dir" ]; then \
			rsync -avz "$(PRJ)/$$dir/" "$(HUGO_SITE)/$$dir/$(PRJ)"; \
		fi; \
	done
	$(RM) -rf "$(HUGO_SITE)/layouts/$(PRJ)/partials"

keep-sync:
	while inotifywait -r -e modify,create,delete $(PROJECTS); do \
		make hugo-sync; \
	done

install-deps:
	sudo apt-get install hugo

.PHONY: clean
clean: hugo-clean
	$(RM) -rv "$(HUGO_SITE)/public"