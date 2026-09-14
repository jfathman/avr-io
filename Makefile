# Makefile

SUBDIRS := core drivers/* programs/* monitor

TOPTARGETS := all clean

$(TOPTARGETS): $(SUBDIRS)

$(SUBDIRS):
	@echo "$@"
	@$(MAKE) --no-print-directory -C $@ $(MAKECMDGOALS)
	@if [ "$(MAKECMDGOALS)" != "clean" ]; then echo ""; fi

.PHONY: $(TOPTARGETS) $(SUBDIRS)
