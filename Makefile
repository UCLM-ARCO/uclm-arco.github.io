# -*- mode: makefile-gmake; coding: utf-8 -*-

HUGO_SITE = hugo-site

# keep this pattern for automatic tools
SECTIONS := arco101
SECTIONS += citisim
SECTIONS += hesperia
SECTIONS += idm
SECTIONS += icec
SECTIONS += symbiot
SECTIONS += recipes
SECTIONS += arco-talks
SECTIONS += api
SECTIONS += cittavr
SECTIONS += shapes
SECTIONS += miratar
# SECTIONS_VAR_END (do not edit or remove this comment)

all run-preview-server run-preview-server-with-drafts run-server:
	$(MAKE) -C $(HUGO_SITE) $@

sync: hugo-clean hugo-sync keep-sync

hugo-sync:
	-@for sect in $(SECTIONS); do \
		make sync-sect sect=$$sect; \
	done

hugo-clean:
	@for dir in content layouts static; do \
		$(RM) -rv $(HUGO_SITE)/$$dir; \
		mkdir -p $(HUGO_SITE)/$$dir; \
	done
	mkdir -p $(HUGO_SITE)/layouts/partials
	cp search/search-index.md $(HUGO_SITE)/content

sync-sect:
	@for dir in content layouts layouts/partials static; do \
		if [ -d "$(sect)/$$dir" ]; then \
			rsync -avz "$(sect)/$$dir/" "$(HUGO_SITE)/$$dir/$(sect)"; \
		fi; \
	done

keep-sync:
	while inotifywait -r -e modify,create,delete $(SECTIONS); do \
		make hugo-sync; \
	done

install-deps:
	sudo apt-get install hugo

.PHONY: clean
clean: hugo-clean
	$(RM) -rv "$(HUGO_SITE)/public"
