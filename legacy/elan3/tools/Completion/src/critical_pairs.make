SUBDIRS = .elan.critical_pairs/

all clean veryclean:
	@for DIR in $(SUBDIRS); do echo Make $@ in $$DIR; cd $$DIR && $(MAKE) $@; cp -f cp.out ..; cd ..; done

