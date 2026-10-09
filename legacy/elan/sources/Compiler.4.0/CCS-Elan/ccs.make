SUBDIRS = .elan.ccs/

all clean veryclean:
	@for DIR in $(SUBDIRS); do echo Make $@ in $$DIR; cd $$DIR && $(MAKE) $@; cp -f libCCS.a ..; cd ..; done

